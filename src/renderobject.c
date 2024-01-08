#include "include/renderobject.h"

RenderObject *createRenderObjectFromFile(ApplicationContext *context, const char *filename, CreateEntityInfo *info) {
    RenderObject *obj = malloc(sizeof(RenderObject));
    obj->scale[0] = info->scaleX;
    obj->scale[1] = info->scaleY;
    obj->scale[2] = info->scaleZ;
    obj->position[0] = info->positionX;
    obj->position[1] = info->positionY;
    obj->position[2] = info->positionZ;
    obj->scale[0] = info->scaleX;
    obj->scale[1] = info->scaleY;
    obj->scale[2] = info->scaleZ;

    obj->modelData = loadModelData(filename);

    // Dynamically allocate a BufferMemory
    obj->transformUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context->vulkanDeviceContext, obj->transformUBO, sizeof(TransformUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Dynamically allocate a BufferMemory
    obj->lightingUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context->vulkanDeviceContext, obj->lightingUBO, sizeof(LightingUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


    // Create vertex buffer with staging
    obj->vertexBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagedBufferMemory(context->vulkanDeviceContext, context->commandPool, obj->vertexBuffer,
                             obj->modelData->num_vertices * sizeof(Vertex),
                             VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                             obj->modelData->vertices);

    // Create index buffer with staging
    obj->indexBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagedBufferMemory(context->vulkanDeviceContext, context->commandPool, obj->indexBuffer,
                             obj->modelData->num_indices * sizeof(unsigned int),
                             VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                             obj->modelData->indices);


    // Upload textures
    obj->colorMap = malloc(sizeof(ImageMemory));
    setupTextureFromImageData(context, obj->modelData->diffuseImageData, obj->colorMap);
    obj->normalMap = malloc(sizeof(ImageMemory));
    setupTextureFromImageData(context, obj->modelData->normalMapImageData, obj->normalMap);
    obj->metallicRoughnessMap = malloc(sizeof(ImageMemory));
    setupTextureFromImageData(context, obj->modelData->metallicRoughnessMapImageData, obj->metallicRoughnessMap);

    // Create descriptor sets
    allocateDescriptorSet(context->vulkanDeviceContext->device, context->pipelineConfig->descriptorPool,
                          context->pipelineConfig->vertexShaderDescriptorSetLayout, &obj->vertexDescriptorSet);
    allocateDescriptorSet(context->vulkanDeviceContext->device, context->pipelineConfig->descriptorPool,
                          context->pipelineConfig->fragmentShaderDescriptorSetLayout, &obj->fragmentDescriptorSet);

    updateBasicShaderDescriptorSet(
            context->vulkanDeviceContext->device,
            obj->vertexDescriptorSet,
            obj->fragmentDescriptorSet,
            obj->transformUBO->buffer,
            obj->lightingUBO->buffer,
            obj->colorMap->imageView,
            obj->normalMap->imageView,
            obj->metallicRoughnessMap->imageView,
            context->sampler
    );

    return obj;
}

void setupTextureFromImageData(ApplicationContext *context, ModelImageData *imageData, ImageMemory *imageMemory) {
    // Create a staging buffer for the image data
    BufferMemory *textureStagingBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagingBufferMemory(context->vulkanDeviceContext, imageData->image_size, imageData->image_data,
                              textureStagingBuffer);

    // Create the Vulkan image for the texture
    createImage(context->vulkanDeviceContext->device, context->vulkanDeviceContext->physicalDevice,
                imageData->image_width, imageData->image_height,
                VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &imageMemory->image, &imageMemory->memory);

    // Transition the image layout and copy the buffer to the image
    transitionTextureImageLayout(context->vulkanDeviceContext->device, context->commandPool,
                                 context->vulkanDeviceContext->graphicsQueue, imageMemory->image,
                                 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(context->vulkanDeviceContext->device, context->commandPool,
                      context->vulkanDeviceContext->graphicsQueue, textureStagingBuffer->buffer,
                      imageMemory->image, imageData->image_width, imageData->image_height);
    transitionTextureImageLayout(context->vulkanDeviceContext->device, context->commandPool,
                                 context->vulkanDeviceContext->graphicsQueue, imageMemory->image,
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Create the image view for the texture
    imageMemory->imageView = createImageView(context->vulkanDeviceContext->device, &imageMemory->image,
                                             VK_FORMAT_R8G8B8A8_UNORM,
                                             VK_IMAGE_ASPECT_COLOR_BIT);

    // Clean up the staging buffer
    destroyBufferMemory(context->vulkanDeviceContext, textureStagingBuffer);
}

void setRenderObjectPosition(RenderObject *obj, float x, float y, float z) {
    obj->position[0] = x;
    obj->position[1] = y;
    obj->position[2] = z;
}

void setRenderObjectRotation(RenderObject *obj, float x, float y, float z) {
    obj->rotation[0] = x;
    obj->rotation[1] = y;
    obj->rotation[2] = z;
}

void destroyRenderObject(ApplicationContext *context, RenderObject *obj) {
    // Destroy UBOs
    destroyBufferMemory(context->vulkanDeviceContext, obj->transformUBO);
    destroyBufferMemory(context->vulkanDeviceContext, obj->lightingUBO);

    // Destroy data buffers
    destroyBufferMemory(context->vulkanDeviceContext, obj->indexBuffer);
    destroyBufferMemory(context->vulkanDeviceContext, obj->vertexBuffer);

    // Destroy maps
    destroyImageMemory(context->vulkanDeviceContext->device, obj->colorMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->normalMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->metallicRoughnessMap);

    // Destroy model data
    destroyModelData(obj->modelData);

    free(obj);
}