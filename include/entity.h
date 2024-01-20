#ifndef APP_ENTITY_H
#define APP_ENTITY_H

#include <cglm/vec3.h>
#include <vulkan/vulkan.h>
#include "aabb.h"
#include "include/vulkan/buffer_memory.h"
#include "renderresources.h"
#include "obb.h"
#include "include/pipelines/basic/basic_pipeline_update_descriptor_sets.h"

typedef struct Entity {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    mat4 modelMatrix;

    AABB aabb;
    bool useOBB;   // Flag to indicate if the entity uses an OBB
    OBB obb;       // The OBB, valid only if useOBB is true

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    BufferMemory *lightingUBO;
    RenderResources *renderResources;

    void *kotlinEntityInfo;
} Entity;

RenderResourcesMap *renderResourcesMap;

Entity *createEntity(ApplicationContext *context, CreateEntityInfo *info);

void setEntityPosition(Entity *entity, float x, float y, float z);

void setEntityRotation(Entity *entity, float x, float y, float z);

void setEntityScale(Entity *obj, float x, float y, float z);

void applyEntityChanges(Entity *entity);

void updateEntityAABB(Entity *entity);

void updateEntityOBB(Entity *entity);

void attachKotlinEntityInfo(Entity *entity, void *kotlinEntityInfo);

void destroyEntity(ApplicationContext *context, Entity *entity);

#endif //APP_ENTITY_H
