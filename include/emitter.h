#ifndef APP_EMITTER_H
#define APP_EMITTER_H

#include <cglm/vec3.h>
#include <vulkan/vulkan.h>
#include "include/vulkan/buffer_memory.h"
#include "renderresources.h"
#include "include/pipelines/compute_pipeline_config.h"

typedef struct Emitter {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    RenderResources *renderResources;

    ComputePipelineConfig *computePipelineConfig;

    // TODO This could be shared between emitters that use the same shaders and compute pipeline
    PipelineConfig *graphicsPipelineConfig;
} Emitter;

RenderResourcesMap *renderResourcesMap;

Emitter *createEmitter(
        ApplicationContext *context,
        const char *filename,
        CreateEmitterInfo *info);
void setEmitterPosition(Emitter *emitter, float x, float y, float z);
void setEmitterRotation(Emitter *emitter, float x, float y, float z);
void setEmitterScale(Emitter *emitter, float x, float y, float z);
void updateEmitterAABB(Emitter *emitter);
void updateEmitterOBB(Emitter* emitter);
void destroyEmitter(ApplicationContext *context, Emitter *emitter);

#endif //APP_EMITTER_H
