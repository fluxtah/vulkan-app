#ifndef APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H
#define APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H

#include <vulkan/vulkan.h>
#include "include/debug.h"

VkPipelineLayout createPfxComputePipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);
#endif //APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H
