#include "include/renderresources.h"

void addRenderResources(RenderResourcesMap **hashmap, const char *filename, RenderResources *resources) {
    RenderResourcesMap *entry = NULL;
    HASH_FIND_STR(*hashmap, filename, entry);
    if (entry == NULL) {
        entry = (RenderResourcesMap *) malloc(sizeof(RenderResourcesMap));
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

AABB calculateAABB(const ModelData *modelData) {
    if (modelData == NULL || modelData->num_vertices == 0) {
        // Handle error: invalid model data
        return (AABB) {.min = {0, 0, 0}, .max = {0, 0, 0}};
    }

    // Initialize min and max with the first vertex
    float min[3] = {
            modelData->vertices[0].position[0],
            modelData->vertices[0].position[1],
            modelData->vertices[0].position[2],
    };
    float max[3] = {
            modelData->vertices[0].position[0],
            modelData->vertices[0].position[1],
            modelData->vertices[0].position[2],
    };

    for (size_t i = 1; i < modelData->num_vertices; ++i) {
        float *pos = modelData->vertices[i].position;

        // Update min and max coordinates
        min[0] = (pos[0] < min[0]) ? pos[0] : min[0];
        min[1] = (pos[1] < min[1]) ? pos[1] : min[1];
        min[2] = (pos[2] < min[2]) ? pos[2] : min[2];

        max[0] = (pos[0] > max[0]) ? pos[0] : max[0];
        max[1] = (pos[1] > max[1]) ? pos[1] : max[1];
        max[2] = (pos[2] > max[2]) ? pos[2] : max[2];
    }

    return (AABB) {
            .min ={min[0], min[1], min[2]},
            .max = {max[0], max[1], max[2]}
    };
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

    if (existingData != NULL) {
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

    // Calculate AABB
    obj->aabb = calculateAABB(obj->modelData);

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