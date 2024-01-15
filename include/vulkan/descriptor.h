#ifndef VULKAN_DESCRIPTOR_H
#define VULKAN_DESCRIPTOR_H

#include "include/ubo.h"
#include "include/debug.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkResult allocateDescriptorSet(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkDescriptorSet *descriptorSet);

#endif // VULKAN_DESCRIPTOR_H