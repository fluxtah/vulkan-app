#include "include/pipelines/pfx/pfx_compute_pipeline_config.h"

ComputePipelineConfig *createPfxComputePipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool) {
    ComputePipelineConfig *config = malloc(sizeof(ComputePipelineConfig));

    config->descriptorPool = createPfxComputePipelineDescriptorPool(context->device);
    if (config->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->descriptorSetLayout = createPfxComputePipelineDescriptorSetLayout(context->device);
    if (config->descriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor set layout for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->pipelineLayout = createPfxComputePipelineLayout(context->device);
    if (config->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->computePipeline = createPfxComputePipeline(context->device, config->pipelineLayout);
    if (config->computePipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->commandBuffers = allocateCommandBuffers(context->device, commandPool, 1);
    if (config->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    return config;
}