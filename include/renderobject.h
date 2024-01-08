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
} RenderData;

typedef struct Entity {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    BufferMemory *lightingUBO;
    RenderData *renderData;
} Entity;

Entity *createEntity(ApplicationContext *context, const char *filename, CreateEntityInfo *info);
RenderData *createRenderDataFromFile(ApplicationContext *context, const char *filename);

void setupTextureFromImageData(ApplicationContext *context, ModelImageData *imageData, ImageMemory *imageMemory);

void setEntityPosition(Entity *entity, float x, float y, float z);

void setEntityRotation(Entity *entity, float x, float y, float z);

void destroyEntity(ApplicationContext *context, Entity *entity);
void destroyRenderData(ApplicationContext *context, RenderData *obj);

#endif //APP_RENDEROBJECT_H
