#ifndef APP_PIPELINES_BASIC_CONFIG_H
#define APP_PIPELINES_BASIC_CONFIG_H
#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/framebuffer.h"
#include "include/pipelines/pipeline_config_shared.h"
#include "include/pipelines/basic/basic_pipeline_layout.h"
#include "include/pipelines/basic/basic_pipeline.h"
#include "include/pipelines/basic/basic_pipeline_renderpass.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);
#endif //APP_PIPELINES_BASIC_CONFIG_H
