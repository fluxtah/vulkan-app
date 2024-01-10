#ifndef APP_RENDEROBJECT_H
#define APP_RENDEROBJECT_H

#include "include/modeldata.h"
#include "include/ubo.h"
#include "include/vulkan/buffer.h"
#include "include/context.h"
#include "include/imagememory.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/image.h"
#include "libs/uthash.h"
#include <cglm/cglm.h>
#include <vulkan/vulkan.h>

typedef struct {
    char *filename;
    ModelData *modelData;
    BufferMemory *vertexBuffer;
    BufferMemory *indexBuffer;
    ImageMemory *colorMap;
    ImageMemory *normalMap;
    ImageMemory *metallicRoughnessMap;
} RenderResources;

typedef struct Entity {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    BufferMemory *lightingUBO;
    RenderResources *renderResources;
} Entity;

Entity *createEntity(ApplicationContext *context, const char *filename, CreateEntityInfo *info);
RenderResources *createRenderResourcesFromFile(ApplicationContext *context, const char *filename);

void setupTextureFromImageData(ApplicationContext *context, ModelImageData *imageData, ImageMemory *imageMemory);

void setEntityPosition(Entity *entity, float x, float y, float z);

void setEntityRotation(Entity *entity, float x, float y, float z);

void setEntityScale(Entity *obj, float x, float y, float z);

void destroyEntity(ApplicationContext *context, Entity *entity);
void destroyRenderResources(ApplicationContext *context, RenderResources *obj);

#endif //APP_RENDEROBJECT_H
