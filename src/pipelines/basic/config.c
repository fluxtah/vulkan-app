#include "include/pipelines/basic/config.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    pipelineConfig->renderPass = createRenderPass(context);
    if (pipelineConfig->renderPass == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create render pass for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->swapChainFramebuffers = createSwapChainFramebuffers(context->device, vulkanSwapchainContext,
                                                                        pipelineConfig->renderPass);
    if (pipelineConfig->swapChainFramebuffers == NULL) {
        LOG_ERROR("Failed to create swap chain framebuffers for debug shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    //
    // Create a descriptor pool
    //
    pipelineConfig->descriptorPool = createBasicShaderDescriptorPool(context->device);
    if (pipelineConfig->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    //
    // Create descriptor set layouts
    //
    pipelineConfig->vertexShaderDescriptorSetLayout = createBasicVertexShaderDescriptorSetLayout(context->device);
    if (pipelineConfig->vertexShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->fragmentShaderDescriptorSetLayout = createBasicFragmentShaderDescriptorSetLayout(context->device);
    if (pipelineConfig->fragmentShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->vertexShaderModule = createShaderModule(context->device, "shaders/basic.vert.spv");
    if (pipelineConfig->vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->fragmentShaderModule = createShaderModule(context->device, "shaders/basic.frag.spv");
    if (pipelineConfig->fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createBasicPipelineLayout(context->device,
                                                               pipelineConfig->vertexShaderDescriptorSetLayout,
                                                               pipelineConfig->fragmentShaderDescriptorSetLayout);

    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
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
            pipelineConfig->renderPass,
            viewport,
            pipelineConfig->vertexShaderModule,
            pipelineConfig->fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->commandBuffers = allocateCommandBuffers(context->device, commandPool, vulkanSwapchainContext->swapChainImageCount);
    if (pipelineConfig->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig, vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    return pipelineConfig;
}
