#ifndef APP_PIPELINES_DEBUG_CONFIG_H
#define APP_PIPELINES_DEBUG_CONFIG_H

#include "include/application_context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/framebuffer.h"
#include "include/pipelines/pipeline_config.h"
#include "include/pipelines/debug/debug_pipeline_layout.h"
#include "include/pipelines/debug/debug_pipeline.h"
#include "include/pipelines/debug/debug_pipeline_renderpass.h"

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context,
        VulkanSwapchainContext *vulkanSwapchainContext,
        VkRenderPass renderPass);

#endif //APP_PIPELINES_DEBUG_CONFIG_H
