#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "context.h"

VkSwapchainKHR createSwapChain(VulkanContext *context);
void createSwapChainImageViews(VulkanContext *context);

#endif // VULKAN_SWAPCHAIN_H
