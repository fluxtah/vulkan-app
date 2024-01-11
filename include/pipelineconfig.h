#ifndef APP_PIPELINECONFIG_H
#define APP_PIPELINECONFIG_H

#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/commandbuffer.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context, VkCommandPool commandPool,
        VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t swapChainImageCount);

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context, VkCommandPool commandPool,
        VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t swapChainImageCount);

void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig);

#endif //APP_PIPELINECONFIG_H
