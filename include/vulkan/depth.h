#ifndef VULKAN_DEPTH_H
#define VULKAN_DEPTH_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "include/context.h"
#include "include/imagememory.h"

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

ImageMemory *createDepthStencil(VulkanDeviceContext *context, VkCommandPool commandPool, VkExtent2D swapChainExtent);

#endif //VULKAN_DEPTH_H
