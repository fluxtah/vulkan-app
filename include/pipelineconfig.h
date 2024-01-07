#ifndef APP_PIPELINECONFIG_H
#define APP_PIPELINECONFIG_H

#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/shaders.h"

struct PipelineConfig *createBasicShaderPipelineConfig(struct ApplicationContext *context, struct VkRenderPass_T *renderPass);
void destroyPipelineConfig(ApplicationContext *context, PipelineConfig *pipelineConfig);

#endif //APP_PIPELINECONFIG_H
