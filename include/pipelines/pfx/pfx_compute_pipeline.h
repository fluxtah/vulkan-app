#ifndef APP_PIPELINES_PFX_COMPUTE_PIPELINE_H
#define APP_PIPELINES_PFX_COMPUTE_PIPELINE_H

#include <vulkan/vulkan.h>
#include "include/debug.h"
#include "include/vulkan/shaders.h"

VkPipeline createPfxComputePipeline(VkDevice device, VkPipelineLayout layout, const char *shaderPath);
#endif //APP_PIPELINES_PFX_COMPUTE_PIPELINE_H
