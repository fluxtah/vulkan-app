#include "include/pipelines/debug/debug_pipeline_config.h"

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context,
        VulkanSwapchainContext *vulkanSwapchainContext,
        VkRenderPass renderPass) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    //
    // For debug pipeline, we don't need a descriptor pool or layouts
    //
    pipelineConfig->descriptorPool = NULL;
    pipelineConfig->vertexShaderDescriptorSetLayout = NULL;
    pipelineConfig->fragmentShaderDescriptorSetLayout = NULL;

    VkShaderModule vertexShaderModule = createShaderModule(context->device, "shaders/debug.vert.spv");
    if (vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for debug shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    VkShaderModule fragmentShaderModule = createShaderModule(context->device, "shaders/debug.frag.spv");
    if (fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for debug shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createDebugPipelineLayout(context->device);
    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for debug shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    Viewport viewport = (Viewport) {
            0, 0,
            (float) vulkanSwapchainContext->swapChainExtent.width,
            (float) vulkanSwapchainContext->swapChainExtent.height,
            0.0f,
            1.0f
    };

    pipelineConfig->pipeline = createDebugPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass,
            viewport,
            vertexShaderModule,
            fragmentShaderModule);

    vkDestroyShaderModule(context->device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context->device, fragmentShaderModule, NULL);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for debug shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    return pipelineConfig;
}
