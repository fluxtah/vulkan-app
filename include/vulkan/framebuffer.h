#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkFramebuffer *createSwapChainFramebuffers(VkDevice device, VkImageView *swapChainImageViews, uint32_t imageCount,
                                           VkRenderPass renderPass, VkExtent2D swapChainExtent, VkImageView depthImageView);
#endif // VULKAN_FRAMEBUFFER_H