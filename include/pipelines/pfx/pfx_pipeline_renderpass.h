#ifndef APP_PIPELINES_PFX_RENDERPASS_H
#define APP_PIPELINES_PFX_RENDERPASS_H
#include <vulkan/vulkan.h>
#include <stdio.h>
#include "include/application_context.h"

VkRenderPass createPfxPipelineRenderPass(VulkanDeviceContext *context);
#endif //APP_PIPELINES_PFX_RENDERPASS_H
