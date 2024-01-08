#ifndef APP_PIPELINECONFIG_H
#define APP_PIPELINECONFIG_H

#include "include/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/shaders.h"

PipelineConfig *createBasicShaderPipelineConfig(VulkanDeviceContext *context, VkRenderPass renderPass, VkExtent2D swapChainExtent);
void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig);

#endif //APP_PIPELINECONFIG_H
