#include "include/pipelines/pipeline_config.h"

void destroyPipelineConfig(
        VulkanDeviceContext *context,
        PipelineConfig *pipelineConfig) {
    if (pipelineConfig->descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(context->device, pipelineConfig->descriptorPool, NULL);
        pipelineConfig->descriptorPool = VK_NULL_HANDLE;
    }

    if (pipelineConfig->vertexShaderDescriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(context->device, pipelineConfig->vertexShaderDescriptorSetLayout, NULL);
        pipelineConfig->vertexShaderDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if (pipelineConfig->fragmentShaderDescriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(context->device, pipelineConfig->fragmentShaderDescriptorSetLayout, NULL);
        pipelineConfig->fragmentShaderDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if (pipelineConfig->pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(context->device, pipelineConfig->pipelineLayout, NULL);
        pipelineConfig->pipelineLayout = VK_NULL_HANDLE;
    }

    if (pipelineConfig->pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(context->device, pipelineConfig->pipeline, NULL);
        pipelineConfig->pipeline = VK_NULL_HANDLE;
    }

    free(pipelineConfig);
}