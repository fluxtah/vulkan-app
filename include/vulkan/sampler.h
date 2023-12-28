#ifndef VULKAN_SAMPLER_H
#define VULKAN_SAMPLER_H

#include "context.h"

#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>

void createTextureSampler(VulkanContext *context, VkSampler *sampler);

#endif //VULKAN_SAMPLER_H
