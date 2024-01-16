#ifndef APP_PIPELINES_BASIC_RENDERPASS_H
#define APP_PIPELINES_BASIC_RENDERPASS_H
#include <vulkan/vulkan.h>
#include <stdio.h>
#include "include/application_context.h"

VkRenderPass createBasicPipelineRenderPass(VulkanDeviceContext *context);
#endif //APP_PIPELINES_BASIC_RENDERPASS_H
