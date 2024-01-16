#ifndef APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_SET_LAYOUT_H
#define APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_SET_LAYOUT_H
#include <vulkan/vulkan.h>
#include "include/debug.h"

VkDescriptorSetLayout createPfxComputePipelineDescriptorSetLayout(VkDevice device);

#endif //APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_SET_LAYOUT_H
