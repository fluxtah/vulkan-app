#ifndef VULKAN_SAMPLER_H
#define VULKAN_SAMPLER_H

#include "include/context.h"

#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>

void createTextureSampler(ApplicationContext *context, VkSampler *sampler);

#endif //VULKAN_SAMPLER_H
