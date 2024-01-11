#include "include/pipelineconfig.h"

PipelineConfig *createBasicShaderPipelineConfig(
        VulkanDeviceContext *context, VkCommandPool commandPool,
        VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t swapChainImageCount) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    //
    // Create a descriptor pool
    //
    pipelineConfig->descriptorPool = createBasicShaderDescriptorPool(context->device);
    if (pipelineConfig->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    //
    // Create descriptor set layouts
    //
    pipelineConfig->vertexShaderDescriptorSetLayout = createBasicVertexShaderDescriptorSetLayout(context->device);
    if (pipelineConfig->vertexShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->fragmentShaderDescriptorSetLayout = createBasicFragmentShaderDescriptorSetLayout(context->device);
    if (pipelineConfig->fragmentShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader descriptor set layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->vertexShaderModule = createShaderModule(context->device, "shaders/basic.vert.spv");
    if (pipelineConfig->vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->fragmentShaderModule = createShaderModule(context->device, "shaders/basic.frag.spv");
    if (pipelineConfig->fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createBasicPipelineLayout(context->device,
                                                               pipelineConfig->vertexShaderDescriptorSetLayout,
                                                               pipelineConfig->fragmentShaderDescriptorSetLayout);

    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    Viewport viewport = (Viewport) {
            0, 0,
            (float) swapChainExtent.width,
            (float) swapChainExtent.height,
            0.0f,
            1.0f
    };
    pipelineConfig->pipeline = createBasicPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass, viewport,
            pipelineConfig->vertexShaderModule,
            pipelineConfig->fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->commandBuffers = allocateCommandBuffers(context->device, commandPool, swapChainImageCount);
    if (pipelineConfig->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    return pipelineConfig;
}

PipelineConfig *createDebugPipelineConfig(
        VulkanDeviceContext *context, VkCommandPool commandPool,
        VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t swapChainImageCount) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    //
    // For debug pipeline, we don't need a descriptor pool or layouts
    //
    pipelineConfig->descriptorPool = NULL;
    pipelineConfig->vertexShaderDescriptorSetLayout = NULL;
    pipelineConfig->fragmentShaderDescriptorSetLayout = NULL;

    pipelineConfig->vertexShaderModule = createShaderModule(context->device, "shaders/debug.vert.spv");
    if (pipelineConfig->vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for debug shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->fragmentShaderModule = createShaderModule(context->device, "shaders/debug.frag.spv");
    if (pipelineConfig->fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for debug shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createDebugPipelineLayout(context->device);
    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for debug shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    Viewport viewport = (Viewport) {
            0, 0,
            (float) swapChainExtent.width,
            (float) swapChainExtent.height,
            0.0f,
            1.0f
    };

    pipelineConfig->pipeline = createDebugPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass, viewport,
            pipelineConfig->vertexShaderModule,
            pipelineConfig->fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for debug shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->commandBuffers = allocateCommandBuffers(context->device, commandPool, swapChainImageCount);
    if (pipelineConfig->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }
    return pipelineConfig;
}


void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig) {
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

    if (pipelineConfig->vertexShaderModule != VK_NULL_HANDLE)
        vkDestroyShaderModule(context->device, pipelineConfig->vertexShaderModule, NULL);

    if (pipelineConfig->fragmentShaderModule != VK_NULL_HANDLE)
        vkDestroyShaderModule(context->device, pipelineConfig->fragmentShaderModule, NULL);

    free(pipelineConfig);
}