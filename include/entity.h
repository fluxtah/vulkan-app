//
// Created by Ian Warwick on 12/01/2024.
//

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <cglm/vec3.h>
#include <vulkan/vulkan.h>
#include "aabb.h"
#include "include/vulkan/buffer_memory.h"
#include "renderresources.h"
#include "obb.h"

typedef struct Entity {
    vec3 scale;
    vec3 position;
    vec3 rotation;
    AABB aabb;
    bool useOBB;   // Flag to indicate if the entity uses an OBB
    OBB obb;       // The OBB, valid only if useOBB is true

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    BufferMemory *lightingUBO;
    RenderResources *renderResources;

    void *kotlinEntity;
} Entity;

RenderResourcesMap *renderResourcesMap;

Entity *createEntity(ApplicationContext *context, const char *filename, CreateEntityInfo *info);
void setEntityPosition(Entity *entity, float x, float y, float z);
void setEntityRotation(Entity *entity, float x, float y, float z);
void setEntityScale(Entity *obj, float x, float y, float z);
void updateEntityAABB(Entity *entity);
void updateEntityOBB(Entity* entity);
void attachKotlinEntity(Entity *entity, void *kotlinEntity);
void destroyEntity(ApplicationContext *context, Entity *entity);

#endif //GAME_ENTITY_H
