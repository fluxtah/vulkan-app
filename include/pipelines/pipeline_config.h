#ifndef APP_PIPELINES_PIPELINE_CONFIG_H
#define APP_PIPELINES_PIPELINE_CONFIG_H

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include "include/vulkan_device_context.h"

typedef struct PipelineConfig {
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout vertexShaderDescriptorSetLayout;
    VkDescriptorSetLayout fragmentShaderDescriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
} PipelineConfig;

void destroyPipelineConfig(
        VulkanDeviceContext *context,
        PipelineConfig *pipelineConfig);

#endif //APP_PIPELINES_PIPELINE_CONFIG_H
