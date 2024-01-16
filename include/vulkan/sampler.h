#ifndef VULKAN_SAMPLER_H
#define VULKAN_SAMPLER_H

#include "include/application_context.h"

#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>

void createTextureSampler(VkDevice device, VkSampler *sampler);

#endif //VULKAN_SAMPLER_H
