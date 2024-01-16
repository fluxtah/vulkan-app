#ifndef APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H
#define APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H
#include <vulkan/vulkan.h>

typedef struct ComputePipelineConfig {
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline computePipeline;
    VkCommandBuffer *commandBuffers;
} ComputePipelineConfig;
#endif //APP_PIPELINES_COMPUTE_PIPELINE_CONFIG_H
