#include "include/pipelines/compute_pipeline_config.h"
#include "include/vulkan/buffer.h"

void destroyComputePipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        ComputePipelineConfig *pipelineConfig) {
    if (pipelineConfig->descriptorPool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(context->device, pipelineConfig->descriptorPool, NULL);
        pipelineConfig->descriptorPool = VK_NULL_HANDLE;
    }

    if (pipelineConfig->descriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(context->device, pipelineConfig->descriptorSetLayout, NULL);
        pipelineConfig->descriptorSetLayout = VK_NULL_HANDLE;
    }

    if (pipelineConfig->pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(context->device, pipelineConfig->pipelineLayout, NULL);
        pipelineConfig->pipelineLayout = VK_NULL_HANDLE;
    }

    if (pipelineConfig->computePipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(context->device, pipelineConfig->computePipeline, NULL);
        pipelineConfig->computePipeline = VK_NULL_HANDLE;
    }

    if (pipelineConfig->commandBuffers != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(context->device, commandPool, 1, pipelineConfig->commandBuffers);
        free(pipelineConfig->commandBuffers);
        pipelineConfig->commandBuffers = VK_NULL_HANDLE;
    }

    if (pipelineConfig->particleBuffer != NULL) {
        destroyBufferMemory(context, pipelineConfig->particleBuffer);
        pipelineConfig->particleBuffer = NULL;
    }

    free(pipelineConfig);
}