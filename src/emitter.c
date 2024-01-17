#include "include/emitter.h"
#include "include/pipelines/pfx/pfx_pipeline_update_descriptor_sets.h"

const int MAX_PARTICLE_COUNT = 100;

Emitter *createEmitter(
        ApplicationContext *context,
        PipelineConfig *pipelineConfig,
        const char *filename,
        CreateEmitterInfo *info) {
    Emitter *emitter = malloc(sizeof(Emitter));

    emitter->scale[0] = info->scaleX;
    emitter->scale[1] = info->scaleY;
    emitter->scale[2] = info->scaleZ;
    emitter->position[0] = info->positionX;
    emitter->position[1] = info->positionY;
    emitter->position[2] = info->positionZ;
    emitter->scale[0] = info->scaleX;
    emitter->scale[1] = info->scaleY;
    emitter->scale[2] = info->scaleZ;

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
            pipelineConfig->descriptorPool,
            pipelineConfig->vertexShaderDescriptorSetLayout,
            &emitter->vertexDescriptorSet) != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate vertex descriptor set");
        return NULL;
    }

    if (allocateDescriptorSet(
            context->vulkanDeviceContext->device,
            pipelineConfig->descriptorPool,
            pipelineConfig->fragmentShaderDescriptorSetLayout,
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

void destroyEmitter(ApplicationContext *context, Emitter *entity) {
    // Destroy UBOs
    destroyBufferMemory(context->vulkanDeviceContext, entity->transformUBO);

    RenderResourcesMap *resources = getRenderResources(renderResourcesMap, entity->renderResources->filename);

    if (resources->refs == 1) {
        deleteRenderResources(&renderResourcesMap, resources);
        destroyRenderResources(context, entity->renderResources);
    } else {
        resources->refs--;
    }
}