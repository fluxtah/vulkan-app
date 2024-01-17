#ifndef APP_PIPELINES_PFX_H
#define APP_PIPELINES_PFX_H

#include "include/pipelines/pipeline_shader_util.h"
#include "include/pipelines/viewport_util.h"
#include "include/vertex.h"
#include "kotlin-game/cinterop/model.h"
#include "include/vulkan/shaders.h"
#include "include/debug.h"
#include "cglm/cglm.h"
#include <stdio.h>

VkPipeline createPfxPipeline(
        VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, Viewport viewport,
        VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);

#endif //APP_PIPELINES_PFX_H
