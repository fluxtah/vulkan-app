#ifndef APP_PIPELINES_BASIC_PIPELINE_RENDERPASS_H
#define APP_PIPELINES_BASIC_PIPELINE_RENDERPASS_H
#include <vulkan/vulkan.h>
#include <stdio.h>
#include "include/context.h"

VkRenderPass createBasicPipelineRenderPass(VulkanDeviceContext *context);
#endif //APP_PIPELINES_BASIC_PIPELINE_RENDERPASS_H
