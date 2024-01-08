#include "include/renderresources.h"

typedef struct RenderResourcesMap {
    char *filename;
    int refs;
    RenderResources *resources;
    UT_hash_handle hh;
} RenderResourcesMap;

static RenderResourcesMap *renderResourcesMap;

void addRenderResources(RenderResourcesMap **hashmap, const char *filename, RenderResources *resources) {
    RenderResourcesMap *entry = NULL;
    HASH_FIND_STR(*hashmap, filename, entry);
    if (entry == NULL) {
        entry = (RenderResourcesMap *)malloc(sizeof(RenderResourcesMap));
        entry->filename = strdup(filename); // Duplicate the filename
        entry->resources = resources;
        entry->refs = 1;
        HASH_ADD_KEYPTR(hh, *hashmap, entry->filename, strlen(entry->filename), entry);
    }
}

RenderResourcesMap *getRenderResources(RenderResourcesMap *hashmap, const char *filename) {
    RenderResourcesMap *entry = NULL;
    HASH_FIND_STR(hashmap, filename, entry);
    return (entry != NULL) ? entry : NULL;
}

void deleteRenderResources(RenderResourcesMap **hashmap, RenderResourcesMap *entry) {
    HASH_DEL(*hashmap, entry);
    free(entry->filename);
    free(entry);
}

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

    entity->renderResources = createRenderResourcesFromFile(context, filename);

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
            entity->renderResources->colorMap->imageView,
            entity->renderResources->normalMap->imageView,
            entity->renderResources->metallicRoughnessMap->imageView,
            context->sampler
    );

    return entity;
}

/**
 * Loads model data from a GLB file and creates render resources. This function utilizes a hash map to manage
 * and share loaded resources. If called with a filename that has been previously
 * loaded, it returns the existing mapped RenderResources object instead of reloading from
 * the file, thus optimizing resource usage.
 *
 * Reference counting is employed to manage the lifecycle of the RenderResources. Each call
 * to this function increments the reference count for the RenderResources associated with the
 * given filename. When an Entity using this RenderResources is destroyed, it decrements the
 * reference count. Once the reference count reaches zero (i.e., no more Entities are using
 * the RenderResources), the RenderResources is deleted to free up resources.
 *
 * Note: Callers are responsible for decrementing the reference count by destroying
 * the Entity when it is no longer needed.
 *
 * @param context  The application context containing Vulkan device and command pool.
 * @param filename The path to the GLB file to load the render data from.
 * @return A pointer to the RenderResources structure containing the loaded data.
 */
RenderResources *createRenderResourcesFromFile(ApplicationContext *context, const char *filename) {
    RenderResourcesMap *existingData = getRenderResources(renderResourcesMap, filename);

    if(existingData != NULL) {
        existingData->refs++;
        return existingData->resources;
    }

    RenderResources *obj = malloc(sizeof(RenderResources));
    addRenderResources(&renderResourcesMap, filename, obj);

    obj->filename = strdup(filename);
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
    // Create a staging buffer for the image resources
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

    RenderResourcesMap *resources = getRenderResources(renderResourcesMap, entity->renderResources->filename);

    if(resources->refs == 1) {
        deleteRenderResources(&renderResourcesMap, resources);
        destroyRenderResources(context, entity->renderResources);
    } else {
        resources->refs--;
    }
}

void destroyRenderResources(ApplicationContext *context, RenderResources *obj) {
    // Destroy resources buffers
    destroyBufferMemory(context->vulkanDeviceContext, obj->indexBuffer);
    destroyBufferMemory(context->vulkanDeviceContext, obj->vertexBuffer);

    // Destroy maps
    destroyImageMemory(context->vulkanDeviceContext->device, obj->colorMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->normalMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->metallicRoughnessMap);

    // Destroy model resources
    destroyModelData(obj->modelData);

    free(obj->filename);

    free(obj);
}