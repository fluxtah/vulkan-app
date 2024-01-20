#include "include/emitter.h"
#include "include/pipelines/pfx/pfx_pipeline_update_descriptor_sets.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_config.h"
#include "include/pipelines/pfx/pfx_pipeline_config.h"

const int MAX_PARTICLE_COUNT = 10;

Emitter *createEmitter(
        ApplicationContext *context,
        CreateEmitterInfo *info) {
    Emitter *emitter = malloc(sizeof(Emitter));

    emitter->computePipelineConfig = createPfxComputePipelineConfig(
            context->vulkanDeviceContext,
            context->commandPool
    );

    emitter->graphicsPipelineConfig = createPfxPipelineConfig(
            context->vulkanDeviceContext,
            context->vulkanSwapchainContext,
            context->renderPass
    );


    emitter->scale[0] = info->scaleX;
    emitter->scale[1] = info->scaleY;
    emitter->scale[2] = info->scaleZ;
    emitter->position[0] = info->positionX;
    emitter->position[1] = info->positionY;
    emitter->position[2] = info->positionZ;
    emitter->rotation[0] = info->rotationX;
    emitter->rotation[1] = info->rotationY;
    emitter->rotation[2] = info->rotationZ;

    // Dynamically allocate a BufferMemory
    emitter->transformUBO = (BufferMemory *) malloc(sizeof(BufferMemory));
    createBufferMemory(
            context->vulkanDeviceContext,
            emitter->transformUBO,
            sizeof(TransformUBO),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    emitter->renderResources = createRenderResourcesFromFile(context, info->modelFileName);

    // Create descriptor sets
    if (allocateDescriptorSet(
            context->vulkanDeviceContext->device,
            emitter->graphicsPipelineConfig->descriptorPool,
            emitter->graphicsPipelineConfig->vertexShaderDescriptorSetLayout,
            &emitter->vertexDescriptorSet) != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate vertex descriptor set");
        return NULL;
    }

    if (allocateDescriptorSet(
            context->vulkanDeviceContext->device,
            emitter->graphicsPipelineConfig->descriptorPool,
            emitter->graphicsPipelineConfig->fragmentShaderDescriptorSetLayout,
            &emitter->fragmentDescriptorSet) != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate fragment descriptor set");
        return NULL;
    }

    updatePfxPipelineDescriptorSets(
            context->vulkanDeviceContext->device,
            emitter->vertexDescriptorSet,
            emitter->fragmentDescriptorSet,
            emitter->transformUBO->buffer,
            emitter->renderResources->colorMap->imageView,
            context->sampler
    );

    return emitter;
}

void setEmitterPosition(Emitter *emitter, float x, float y, float z) {
    emitter->position[0] = x;
    emitter->position[1] = y;
    emitter->position[2] = z;
}

void setEmitterRotation(Emitter *emitter, float x, float y, float z) {
    emitter->rotation[0] = x;
    emitter->rotation[1] = y;
    emitter->rotation[2] = z;
}

void setEmitterScale(Emitter *emitter, float x, float y, float z) {
    emitter->scale[0] = x;
    emitter->scale[1] = y;
    emitter->scale[2] = z;
}

void applyEmitterChanges(Emitter *emitter) {
    glm_mat4_identity(emitter->modelMatrix);

    // Apply rotation and translation first
    glm_translate(emitter->modelMatrix, emitter->position);
    glm_rotate(emitter->modelMatrix, glm_rad(emitter->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(emitter->modelMatrix, glm_rad(emitter->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(emitter->modelMatrix, glm_rad(emitter->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    // Then apply non-uniform scaling
    glm_scale(emitter->modelMatrix, emitter->scale);
}

void destroyEmitter(ApplicationContext *context, Emitter *emitter) {
    destroyComputePipelineConfig(
            context->vulkanDeviceContext,
            context->commandPool,
            emitter->computePipelineConfig);

    destroyPipelineConfig(
            context->vulkanDeviceContext,
            emitter->graphicsPipelineConfig);

    // Destroy UBOs
    destroyBufferMemory(context->vulkanDeviceContext, emitter->transformUBO);

    RenderResourcesMap *resources = getRenderResources(renderResourcesMap, emitter->renderResources->filename);

    if (resources->refs == 1) {
        deleteRenderResources(&renderResourcesMap, resources);
        destroyRenderResources(context, emitter->renderResources);
    } else {
        resources->refs--;
    }
}