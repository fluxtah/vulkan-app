#ifndef APP_PIPELINES_BASIC_DESCRIPTOR_POOL_H
#define APP_PIPELINES_BASIC_DESCRIPTOR_POOL_H

#include <vulkan/vulkan.h>
#include "include/debug.h"

VkDescriptorPool createBasicPipelineDescriptorPool(VkDevice device);

#endif //APP_PIPELINES_BASIC_DESCRIPTOR_POOL_H
