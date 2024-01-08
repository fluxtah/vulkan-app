#include "include/renderobject.h"

typedef struct RenderDataMap {
    char *filename;
    int refs;
    RenderData *data;
    UT_hash_handle hh;
} RenderDataMap;

static RenderDataMap *renderDataMap;

void addRenderData(RenderDataMap **hashmap, const char *filename, RenderData *renderObj) {
    RenderDataMap *entry = NULL;
    HASH_FIND_STR(*hashmap, filename, entry);
    if (entry == NULL) {
        entry = (RenderDataMap *)malloc(sizeof(RenderDataMap));
        entry->filename = strdup(filename); // Duplicate the filename
        entry->data = renderObj;
        entry->refs = 1;
        HASH_ADD_KEYPTR(hh, *hashmap, entry->filename, strlen(entry->filename), entry);
    }
}

RenderDataMap *getRenderData(RenderDataMap *hashmap, const char *filename) {
    RenderDataMap *entry = NULL;
    HASH_FIND_STR(hashmap, filename, entry);
    return (entry != NULL) ? entry : NULL;
}

void deleteRenderData(RenderDataMap **hashmap, RenderDataMap *entry) {
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

    entity->renderData = createRenderDataFromFile(context, filename);

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
            entity->renderData->colorMap->imageView,
            entity->renderData->normalMap->imageView,
            entity->renderData->metallicRoughnessMap->imageView,
            context->sampler
    );

    return entity;
}

RenderData *createRenderDataFromFile(ApplicationContext *context, const char *filename) {
    RenderDataMap *existingData = getRenderData(renderDataMap, filename);

    // TODO Caller should be aware that we count refs!
    if(existingData != NULL) {
        existingData->refs++;
        return existingData->data;
    }

    RenderData *obj = malloc(sizeof(RenderData));
    addRenderData(&renderDataMap, filename, obj);

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

    RenderDataMap *renderData = getRenderData(renderDataMap, entity->renderData->filename);

    if(renderData->refs == 1) {
        destroyRenderData(context, entity->renderData);
    } else {
        renderData->refs--;
    }
}

void destroyRenderData(ApplicationContext *context, RenderData *obj) {
    // Destroy data buffers
    destroyBufferMemory(context->vulkanDeviceContext, obj->indexBuffer);
    destroyBufferMemory(context->vulkanDeviceContext, obj->vertexBuffer);

    // Destroy maps
    destroyImageMemory(context->vulkanDeviceContext->device, obj->colorMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->normalMap);
    destroyImageMemory(context->vulkanDeviceContext->device, obj->metallicRoughnessMap);

    // Destroy model data
    destroyModelData(obj->modelData);

    free(obj->filename);

    free(obj);
}