#ifndef APP_PIPELINES_DEBUG_PIPELINE_H
#define APP_PIPELINES_DEBUG_PIPELINE_H

#include "include/pipelines/pipeline_shader_util.h"
#include "include/pipelines/viewport_util.h"
#include "include/vertex.h"
#include "kotlin-game/cinterop/model.h"
#include <stdio.h>

VkPipeline createDebugPipeline(
        VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, Viewport viewport,
        VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);

#endif //APP_PIPELINES_DEBUG_PIPELINE_H
