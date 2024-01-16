#include "include/pipelines/debug/debug_pipeline_config.h"

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        VulkanSwapchainContext *vulkanSwapchainContext) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    pipelineConfig->renderPass = createDebugRenderPass(context);
    if (pipelineConfig->renderPass == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create render pass for basic shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->swapChainFramebuffers = createSwapChainFramebuffers(context->device, vulkanSwapchainContext,
                                                                        pipelineConfig->renderPass);
    if (pipelineConfig->swapChainFramebuffers == NULL) {
        LOG_ERROR("Failed to create swap chain framebuffers for debug shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }


    //
    // For debug pipeline, we don't need a descriptor pool or layouts
    //
    pipelineConfig->descriptorPool = NULL;
    pipelineConfig->vertexShaderDescriptorSetLayout = NULL;
    pipelineConfig->fragmentShaderDescriptorSetLayout = NULL;

    VkShaderModule vertexShaderModule = createShaderModule(context->device, "shaders/debug.vert.spv");
    if (vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for debug shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    VkShaderModule fragmentShaderModule = createShaderModule(context->device, "shaders/debug.frag.spv");
    if (fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for debug shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createDebugPipelineLayout(context->device);
    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for debug shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
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
            pipelineConfig->renderPass, viewport,
            vertexShaderModule,
            fragmentShaderModule);

    vkDestroyShaderModule(context->device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context->device, fragmentShaderModule, NULL);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for debug shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }

    pipelineConfig->commandBuffers = allocateCommandBuffers(context->device, commandPool,
                                                            vulkanSwapchainContext->swapChainImageCount);
    if (pipelineConfig->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyPipelineConfig(
                context,
                commandPool,
                pipelineConfig,
                vulkanSwapchainContext->swapChainImageCount);
        return NULL;
    }
    return pipelineConfig;
}
