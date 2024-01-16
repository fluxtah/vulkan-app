#ifndef APP_RENDEROBJECT_H
#define APP_RENDEROBJECT_H

#include "include/modeldata.h"
#include "include/ubo.h"
#include "include/vulkan/buffer.h"
#include "include/application_context.h"
#include "include/imagememory.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/image.h"
#include "libs/uthash.h"
#include "aabb.h"
#include <cglm/cglm.h>
#include <vulkan/vulkan.h>

typedef struct RenderResources {
    char *filename;
    ModelData *modelData;
    BufferMemory *vertexBuffer;
    BufferMemory *indexBuffer;
    ImageMemory *colorMap;
    ImageMemory *normalMap;
    ImageMemory *metallicRoughnessMap;
    AABB aabb;
} RenderResources;

typedef struct RenderResourcesMap {
    char *filename;
    int refs;
    RenderResources *resources;
    UT_hash_handle hh;
} RenderResourcesMap;

extern RenderResourcesMap *renderResourcesMap;

RenderResources *createRenderResourcesFromFile(ApplicationContext *context, const char *filename);

void setupTextureFromImageData(ApplicationContext *context, ModelImageData *imageData, ImageMemory *imageMemory);

void destroyRenderResources(ApplicationContext *context, RenderResources *obj);

RenderResourcesMap *getRenderResources(RenderResourcesMap *hashmap, const char *filename);
void deleteRenderResources(RenderResourcesMap **hashmap, RenderResourcesMap *entry);

#endif //APP_RENDEROBJECT_H
