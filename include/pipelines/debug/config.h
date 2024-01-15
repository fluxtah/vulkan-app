#ifndef APP_PIPELINES_DEBUG_CONFIG_H
#define APP_PIPELINES_DEBUG_CONFIG_H

#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/framebuffer.h"
#include "include/pipelines/shared.h"
#include "include/pipelines/debug/pipeline_layout.h"
#include "include/pipelines/debug/pipeline.h"

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);

#endif //APP_PIPELINES_DEBUG_CONFIG_H
