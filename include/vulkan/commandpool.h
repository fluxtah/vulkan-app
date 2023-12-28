#ifndef VULKAN_COMMANDPOOL_H
#define VULKAN_COMMANDPOOL_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex);

#endif // VULKAN_COMMANDPOOL_H