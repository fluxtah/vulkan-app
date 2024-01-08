#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/context.h"

VkFramebuffer *createSwapChainFramebuffers(VkDevice device, VulkanSwapchainContext *vulkanSwapchainContext);

#endif // VULKAN_FRAMEBUFFER_H