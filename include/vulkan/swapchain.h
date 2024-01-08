#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/context.h"

typedef struct CreateSwapChainResult {
    VkSwapchainKHR swapChain;
    VkExtent2D swapChainExtent;
} CreateSwapChainResult;

int createSwapChain(VulkanDeviceContext *vulkanDeviceContext, CreateSwapChainResult *swapChainResult);
VkImageView* createSwapChainImageViews(VkDevice device, VulkanSwapchainContext *context);

#endif // VULKAN_SWAPCHAIN_H
