#ifndef VULKAN_DEPTH_H
#define VULKAN_DEPTH_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "context.h"

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
void createDepthResources(VulkanContext *context, VkExtent2D swapChainExtent, VkCommandPool commandPool, VkImage *depthImage, VkDeviceMemory *depthImageMemory, VkImageView *depthImageView);
#endif //VULKAN_DEPTH_H
