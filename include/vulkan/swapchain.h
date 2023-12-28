#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "context.h"

VkSwapchainKHR createSwapChain(VulkanContext *context, VkExtent2D *swapchainExtent);
void createSwapChainImageViews(VkDevice device, VkSwapchainKHR swapChain, VkImageView **swapChainImageViews, uint32_t *imageCount);

#endif // VULKAN_SWAPCHAIN_H
