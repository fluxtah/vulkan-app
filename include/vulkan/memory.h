#ifndef VULKAN_MEMORY_H
#define VULKAN_MEMORY_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

#endif //VULKAN_MEMORY_H
