#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/context.h"

VkFramebuffer *createSwapChainFramebuffers(ApplicationContext *context, VkImageView *swapChainImageViews, uint32_t imageCount,
                                           VkRenderPass renderPass, VkImageView depthImageView);
#endif // VULKAN_FRAMEBUFFER_H