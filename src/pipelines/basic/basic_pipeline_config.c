#include "include/pipelines/basic/basic_pipeline_config.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext,
        VkRenderPass renderPass) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));
    //
    // Create a descriptor pool
    //
    pipelineConfig->descriptorPool = createBasicPipelineDescriptorPool(context->device);
    if (pipelineConfig->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    //
    // Create descriptor set layouts
    //
    pipelineConfig->vertexShaderDescriptorSetLayout = createBasicPipelineVertexShaderDescriptorSetLayout(
            context->device);
    if (pipelineConfig->vertexShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    pipelineConfig->fragmentShaderDescriptorSetLayout = createBasicPipelineFragmentShaderDescriptorSetLayout(
            context->device);
    if (pipelineConfig->fragmentShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    VkShaderModule vertexShaderModule = createShaderModule(context->device, "shaders/basic.vert.spv");
    if (vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    VkShaderModule fragmentShaderModule = createShaderModule(context->device, "shaders/basic.frag.spv");
    if (fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createBasicPipelineLayout(context->device,
                                                               pipelineConfig->vertexShaderDescriptorSetLayout,
                                                               pipelineConfig->fragmentShaderDescriptorSetLayout);

    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for basic shader pipeline");
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
    pipelineConfig->pipeline = createBasicPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass,
            viewport,
            vertexShaderModule,
            fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for basic shader pipeline");
        destroyPipelineConfig(context,pipelineConfig);
        return NULL;
    }

    vkDestroyShaderModule(context->device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context->device, fragmentShaderModule, NULL);

    return pipelineConfig;
}
