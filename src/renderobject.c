#include "include/renderobject.h"

Entity *createEntity(ApplicationContext *context, const char *filename, CreateEntityInfo *info) {
    Entity *entity = malloc(sizeof(Entity));

    entity->scale[0] = info->scaleX;
    entity->scale[1] = info->scaleY;
    entity->scale[2] = info->scaleZ;
    entity->position[0] = info->positionX;
    entity->position[1] = info->positionY;
    entity->position[2] = info->positionZ;
    entity->scale[0] = info->scaleX;
    entity->scale[1] = info->scaleY;
    entity->scale[2] = info->scaleZ;

    // Dynamically allocate a BufferMemory
    entity->transformUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context->vulkanDeviceContext, entity->transformUBO, sizeof(TransformUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Dynamically allocate a BufferMemory
    entity->lightingUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context->vulkanDeviceContext, entity->lightingUBO, sizeof(LightingUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    entity->renderObject = createRenderObjectFromFile(context, filename);

    // Create descriptor sets
    allocateDescriptorSet(context->vulkanDeviceContext->device, context->pipelineConfig->descriptorPool,
                          context->pipelineConfig->vertexShaderDescriptorSetLayout, &entity->vertexDescriptorSet);
    allocateDescriptorSet(context->vulkanDeviceContext->device, context->pipelineConfig->descriptorPool,
                          context->pipelineConfig->fragmentShaderDescriptorSetLayout, &entity->fragmentDescriptorSet);

    updateBasicShaderDescriptorSet(
            context->vulkanDeviceContext->device,
            entity->vertexDescriptorSet,
            entity->fragmentDescriptorSet,
            entity->transformUBO->buffer,
            entity->lightingUBO->buffer,
            entity->renderObject->colorMap->imageView,
            entity->renderObject->normalMap->imageView,
            entity->renderObject->metallicRoughnessMap->imageView,
            context->sampler
    );

    return entity;
}

RenderObject *createRenderObjectFromFile(ApplicationContext *context, const char *filename) {
    RenderObject *obj = malloc(sizeof(RenderObject));

    obj->modelData = loadModelData(filename);

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

void setEntityPosition(Entity *obj, float x, float y, float z) {
    obj->position[0] = x;
    obj->position[1] = y;
    obj->position[2] = z;
}

void setEntityRotation(Entity *obj, float x, float y, float z) {
    obj->rotation[0] = x;
    obj->rotation[1] = y;
    obj->rotation[2] = z;
}

void destroyEntity(ApplicationContext *context, Entity *entity) {
    // Destroy UBOs
    destroyBufferMemory(context->vulkanDeviceContext, entity->transformUBO);
    destroyBufferMemory(context->vulkanDeviceContext, entity->lightingUBO);


    int refs = 1;
    // TODO Store render objects in a hash map and increment/decrement reference count
    if (refs == 1) {
        destroyRenderObject(context, entity->renderObject);
    }
}

void destroyRenderObject(ApplicationContext *context, RenderObject *obj) {
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