#ifndef APP_EMITTER_H
#define APP_EMITTER_H

#include <cglm/vec3.h>
#include <vulkan/vulkan.h>
#include "include/vulkan/buffer_memory.h"
#include "renderresources.h"

typedef struct Emitter {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;
    BufferMemory *transformUBO;
    RenderResources *renderResources;
} Emitter;

RenderResourcesMap *renderResourcesMap;

Emitter *createEmitter(
        ApplicationContext *context,
        PipelineConfig *pipelineConfig,
        const char *filename,
        CreateEmitterInfo *info);
void setEmitterPosition(Emitter *emitter, float x, float y, float z);
void setEmitterRotation(Emitter *emitter, float x, float y, float z);
void setEmitterScale(Emitter *emitter, float x, float y, float z);
void updateEmitterAABB(Emitter *emitter);
void updateEmitterOBB(Emitter* emitter);
void destroyEmitter(ApplicationContext *context, Emitter *emitter);

#endif //APP_EMITTER_H
