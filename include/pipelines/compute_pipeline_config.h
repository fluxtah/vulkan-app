#ifndef APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H
#define APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H

#include <vulkan/vulkan.h>
#include "include/vulkan_device_context.h"
#include "include/vulkan/buffer_memory.h"
#include <stdlib.h>

typedef struct ComputePipelineConfig {
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline computePipeline;
    VkCommandBuffer *commandBuffers;
    BufferMemory *particleBuffer;
} ComputePipelineConfig;

void destroyComputePipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        ComputePipelineConfig *pipelineConfig);

#endif //APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H
