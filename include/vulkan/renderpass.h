#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include "include/context.h"
#include <stdio.h>
#include <vulkan/vulkan.h>

VkRenderPass createRenderPass(VulkanDeviceContext *context);

#endif // VULKAN_RENDERPASS_H