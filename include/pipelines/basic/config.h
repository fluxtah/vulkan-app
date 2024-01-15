#ifndef APP_PIPELINES_BASIC_CONFIG_H
#define APP_PIPELINES_BASIC_CONFIG_H
#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/framebuffer.h"
#include "include/pipelines/shared.h"
#include "include/pipelines/basic/pipeline_layout.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);
#endif //APP_PIPELINES_BASIC_CONFIG_H
