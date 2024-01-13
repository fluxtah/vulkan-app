//
// Created by Ian Warwick on 12/01/2024.
//
#include "include/entity.h"

Entity *createEntity(ApplicationContext *context, const char *filename, CreateEntityInfo *info) {
    Entity *entity = malloc(sizeof(Entity));

    entity->useOBB = info->useOrientedBoundingBox;
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
    if (allocateDescriptorSet(
            context->vulkanDeviceContext->device,
            context->pipelineConfig->descriptorPool,
            context->pipelineConfig->vertexShaderDescriptorSetLayout,
            &entity->vertexDescriptorSet) != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate vertex descriptor set");
        return NULL;
    }

    if (allocateDescriptorSet(
            context->vulkanDeviceContext->device,
            context->pipelineConfig->descriptorPool,
            context->pipelineConfig->fragmentShaderDescriptorSetLayout,
            &entity->fragmentDescriptorSet)) {
        LOG_ERROR("Failed to allocate fragment descriptor set");
        return NULL;
    }

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

    entity->aabb = entity->renderResources->aabb;

    if(entity->useOBB)
        updateEntityOBB(entity);
    else
        updateEntityAABB(entity);

    return entity;
}

void setEntityPosition(Entity *obj, float x, float y, float z) {
    obj->position[0] = x;
    obj->position[1] = y;
    obj->position[2] = z;

    if(obj->useOBB)
        updateEntityOBB(obj);
    else
        updateEntityAABB(obj);
}

void setEntityRotation(Entity *obj, float x, float y, float z) {
    obj->rotation[0] = x;
    obj->rotation[1] = y;
    obj->rotation[2] = z;

    if(obj->useOBB)
        updateEntityOBB(obj);
}

void setEntityScale(Entity *obj, float x, float y, float z) {
    obj->scale[0] = x;
    obj->scale[1] = y;
    obj->scale[2] = z;

    if(obj->useOBB)
        updateEntityOBB(obj);
    else
        updateEntityAABB(obj);
}

void updateEntityAABB(Entity *entity) {
    // Assuming original AABB is stored in the entity
    AABB originalAABB = entity->renderResources->aabb;

    // Scale the AABB
    vec3 scaledMin, scaledMax;
    glm_vec3_mul(originalAABB.min, entity->scale, scaledMin);
    glm_vec3_mul(originalAABB.max, entity->scale, scaledMax);

    // Translate the AABB
    glm_vec3_add(scaledMin, entity->position, entity->aabb.min);
    glm_vec3_add(scaledMax, entity->position, entity->aabb.max);
}

void updateEntityOBB(Entity* entity) {
    AABB aabb = entity->renderResources->aabb;

    // Calculate the original center and extents of the AABB
    glm_vec3_add(aabb.min, aabb.max, entity->obb.center);
    glm_vec3_scale(entity->obb.center, 0.5f, entity->obb.center);
    glm_vec3_sub(aabb.max, entity->obb.center, entity->obb.extents);

    // Create rotation-translation matrix
    mat4 rotTransMatrix;
    glm_mat4_identity(rotTransMatrix);

    // Translate to the position, including the original center of the AABB
    vec3 finalPosition;
    glm_vec3_add(entity->position, entity->obb.center, finalPosition);
    glm_translate(rotTransMatrix, finalPosition);

    // Apply rotation
    glm_rotate(rotTransMatrix, glm_rad(entity->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(rotTransMatrix, glm_rad(entity->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(rotTransMatrix, glm_rad(entity->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    // Create a separate scaling matrix
    mat4 scaleMatrix;
    glm_mat4_identity(scaleMatrix);
    glm_scale(scaleMatrix, entity->scale);

    // Combine the scale matrix with the rotation-translation matrix
    mat4 transformMatrix;
    glm_mat4_mul(rotTransMatrix, scaleMatrix, transformMatrix);

    // Store the combined transformation matrix for rendering the OBB
    glm_mat4_copy(transformMatrix, entity->obb.transform);
}

void attachKotlinEntity(Entity *entity, void *kotlinEntity) {
    entity->kotlinEntity = kotlinEntity;
}

void destroyEntity(ApplicationContext *context, Entity *entity) {
    // Destroy UBOs
    destroyBufferMemory(context->vulkanDeviceContext, entity->transformUBO);
    destroyBufferMemory(context->vulkanDeviceContext, entity->lightingUBO);

    RenderResourcesMap *resources = getRenderResources(renderResourcesMap, entity->renderResources->filename);

    if (resources->refs == 1) {
        deleteRenderResources(&renderResourcesMap, resources);
        destroyRenderResources(context, entity->renderResources);
    } else {
        resources->refs--;
    }
}


