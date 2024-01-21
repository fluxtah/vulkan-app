#ifndef APP_PIPELINES_PFX_PIPELINE_CONFIG_H
#define APP_PIPELINES_PFX_PIPELINE_CONFIG_H

#include <vulkan/vulkan.h>
#include "include/application_context.h"

PipelineConfig *createPfxPipelineConfig(
        VulkanDeviceContext *context,
        VulkanSwapchainContext *vulkanSwapchainContext,
        VkRenderPass renderPass,
        const char *vertexShaderPath,
        const char *fragmentShaderPath);

#endif //APP_PIPELINES_PFX_PIPELINE_CONFIG_H
