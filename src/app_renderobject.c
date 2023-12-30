#include "include/app_renderobject.h"

RenderObject *createRenderObjectFromFile(
        VulkanContext *context,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout,
        VkDescriptorPool descriptorPool,
        const char *filename) {
    RenderObject *obj = malloc(sizeof(RenderObject));
    obj->scale[0] = 1.0f;
    obj->scale[1] = 1.0f;
    obj->scale[2] = 1.0f;
    obj->modelData = loadModelData(filename);

    // Dynamically allocate a BufferMemory
    obj->transformUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context, obj->transformUBO, sizeof(TransformUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Dynamically allocate a BufferMemory
    obj->lightArrayUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(context, obj->lightArrayUBO, sizeof(LightArrayUBO),
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


    // Create vertex buffer with staging
    obj->vertexBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagedBufferMemory(context, context->commandPool, obj->vertexBuffer,
                             obj->modelData->num_vertices * sizeof(Vertex),
                             VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                             obj->modelData->vertices);

    // Create index buffer with staging
    obj->indexBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagedBufferMemory(context, context->commandPool, obj->indexBuffer,
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

    // Create a descriptor sets
    allocateDescriptorSet(context->device, descriptorPool, vertexDescriptorSetLayout, &obj->vertexDescriptorSet);
    allocateDescriptorSet(context->device, descriptorPool, fragmentDescriptorSetLayout, &obj->fragmentDescriptorSet);

    updateBasicShaderDescriptorSet(
            context->device,
            obj->vertexDescriptorSet,
            obj->fragmentDescriptorSet,
            obj->transformUBO->buffer,
            obj->lightArrayUBO->buffer,
            obj->colorMap->imageView,
            obj->normalMap->imageView,
            obj->metallicRoughnessMap->imageView,
            context->sampler
    );

    return obj;
}

void setupTextureFromImageData(VulkanContext *context, ModelImageData *imageData, ImageMemory *imageMemory) {
    // Create a staging buffer for the image data
    BufferMemory *textureStagingBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));
    createStagingBufferMemory(context, imageData->image_size, imageData->image_data, textureStagingBuffer);

    // Create the Vulkan image for the texture
    createImage(context->device, context->physicalDevice, imageData->image_width, imageData->image_height,
                VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &imageMemory->image, &imageMemory->memory);

    // Transition the image layout and copy the buffer to the image
    transitionTextureImageLayout(context->device, context->commandPool, context->graphicsQueue, imageMemory->image,
                                 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(context->device, context->commandPool, context->graphicsQueue, textureStagingBuffer->buffer,
                      imageMemory->image, imageData->image_width, imageData->image_height);
    transitionTextureImageLayout(context->device, context->commandPool, context->graphicsQueue, imageMemory->image,
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Create the image view for the texture
    imageMemory->imageView = createImageView(context->device, &imageMemory->image, VK_FORMAT_R8G8B8A8_UNORM,
                                             VK_IMAGE_ASPECT_COLOR_BIT);

    // Clean up the staging buffer
    destroyBufferMemory(context, textureStagingBuffer);
}


void destroyRenderObject(VulkanContext *context, RenderObject *obj) {
    // Destroy UBO's
    destroyBufferMemory(context, obj->transformUBO);
    destroyBufferMemory(context, obj->lightArrayUBO);

    // Destroy data buffers
    destroyBufferMemory(context, obj->indexBuffer);
    destroyBufferMemory(context, obj->vertexBuffer);

    // Destroy color map
    vkDestroyImageView(context->device, obj->colorMap->imageView, NULL);
    vkDestroyImage(context->device, obj->colorMap->image, NULL);
    vkFreeMemory(context->device, obj->colorMap->memory, NULL);
    free(obj->colorMap);

    // Destroy normal map
    vkDestroyImageView(context->device, obj->normalMap->imageView, NULL);
    vkDestroyImage(context->device, obj->normalMap->image, NULL);
    vkFreeMemory(context->device, obj->normalMap->memory, NULL);
    free(obj->normalMap);

    // Destroy metallic roughness map
    vkDestroyImageView(context->device, obj->metallicRoughnessMap->imageView, NULL);
    vkDestroyImage(context->device, obj->metallicRoughnessMap->image, NULL);
    vkFreeMemory(context->device, obj->metallicRoughnessMap->memory, NULL);
    free(obj->metallicRoughnessMap);

    // Destroy model data
    destroyModelData(obj->modelData);

    free(obj);
}