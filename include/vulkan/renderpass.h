#ifndef VULKAN_RENDERPASS_H
#define VULKAN_RENDERPASS_H

#include "include/context.h"
#include <stdio.h>
#include <vulkan/vulkan.h>

VkRenderPass createRenderPass(ApplicationContext *context);
void destroyRenderPass(ApplicationContext *context, VkRenderPass renderPass);
#endif // VULKAN_RENDERPASS_H