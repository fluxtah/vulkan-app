#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include "context.h"
#include <stdio.h>
#include <vulkan/vulkan.h>

VkRenderPass createRenderPass(VulkanContext *context);
void destroyRenderPass(VulkanContext *context, VkRenderPass renderPass);
#endif // VULKAN_RENDERPASS_H