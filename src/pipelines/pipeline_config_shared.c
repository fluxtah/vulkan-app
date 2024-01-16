#include "include/pipelines/pipeline_config_shared.h"

void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig,
                           uint32_t swapChainImageCount) {
    if (pipelineConfig->descriptorPool != VK_NULL_HANDLE)
        vkDestroyDescriptorPool(context->device, pipelineConfig->descriptorPool, NULL);

    if (pipelineConfig->vertexShaderDescriptorSetLayout != VK_NULL_HANDLE)
        vkDestroyDescriptorSetLayout(context->device, pipelineConfig->vertexShaderDescriptorSetLayout, NULL);

    if (pipelineConfig->fragmentShaderDescriptorSetLayout != VK_NULL_HANDLE)
        vkDestroyDescriptorSetLayout(context->device, pipelineConfig->fragmentShaderDescriptorSetLayout, NULL);

    if (pipelineConfig->pipelineLayout != VK_NULL_HANDLE)
        vkDestroyPipelineLayout(context->device, pipelineConfig->pipelineLayout, NULL);

    if (pipelineConfig->pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(context->device, pipelineConfig->pipeline, NULL);

    if (pipelineConfig->renderPass != VK_NULL_HANDLE)
        vkDestroyRenderPass(context->device, pipelineConfig->renderPass, NULL);

    for (size_t i = 0; i < swapChainImageCount; i++) {
        vkDestroyFramebuffer(context->device, pipelineConfig->swapChainFramebuffers[i], NULL);
    }
    free(pipelineConfig->swapChainFramebuffers);

    free(pipelineConfig);
}
