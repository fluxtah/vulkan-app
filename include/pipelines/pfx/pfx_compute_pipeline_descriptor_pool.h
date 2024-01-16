#ifndef APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_POOL_H
#define APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_POOL_H

#include <vulkan/vulkan.h>
#include "include/debug.h"

VkDescriptorPool createPfxComputePipelineDescriptorPool(VkDevice device);

#endif //APP_PFX_COMPUTE_PIPELINE_DESCRIPTOR_POOL_H
