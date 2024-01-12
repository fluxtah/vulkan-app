#ifndef APP_PIPELINECONFIG_H
#define APP_PIPELINECONFIG_H

#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/framebuffer.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext);

void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig, uint32_t swapChainImageCount);

#endif //APP_PIPELINECONFIG_H
