#include "include/emitter.h"
#include "include/pipelines/pfx/pfx_pipeline_update_descriptor_sets.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_config.h"
#include "include/pipelines/pfx/pfx_pipeline_config.h"

const int MAX_PARTICLE_COUNT = 20;

Emitter *createEmitter(
        ApplicationContext *context,
        const char *filename,
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

    emitter->renderResources = createRenderResourcesFromFile(context, filename);

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