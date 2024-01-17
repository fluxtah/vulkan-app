#ifndef APP_PIPELINES_PFX_PIPELINE_CONFIG_H
#define APP_PIPELINES_PFX_PIPELINE_CONFIG_H

#include <vulkan/vulkan.h>
#include "include/application_context.h"

PipelineConfig *createPfxPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);

#endif //APP_PIPELINES_PFX_PIPELINE_CONFIG_H
