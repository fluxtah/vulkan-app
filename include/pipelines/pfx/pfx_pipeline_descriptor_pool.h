#ifndef APP_PIPELINES_PFX_DESCRIPTOR_POOL_H
#define APP_PIPELINES_PFX_DESCRIPTOR_POOL_H

#include <vulkan/vulkan.h>
#include "include/debug.h"

VkDescriptorPool createPfxPipelineDescriptorPool(VkDevice device);

#endif //APP_PIPELINES_PFX_DESCRIPTOR_POOL_H
