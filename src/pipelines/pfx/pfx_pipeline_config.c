#include "include/pipelines/pfx/pfx_pipeline_config.h"
#include "include/pipelines/pfx/pfx_pipeline_descriptor_pool.h"
#include "include/pipelines/pfx/pfx_pipeline_vertex_descriptor_set_layout.h"
#include "include/vulkan/shaders.h"
#include "include/pipelines/pfx/pfx_pipeline_frag_descriptor_set_layout.h"
#include "include/pipelines/pfx/pfx_pipeline_layout.h"
#include "include/pipelines/pfx/pfx_pipeline.h"

PipelineConfig *createPfxPipelineConfig(
        VulkanDeviceContext *context,
        VulkanSwapchainContext *vulkanSwapchainContext,
        VkRenderPass renderPass,
        const char *vertexShaderPath,
        const char *fragmentShaderPath) {
    PipelineConfig *pipelineConfig = malloc(sizeof(PipelineConfig));

    //
    // Create a descriptor pool
    //
    pipelineConfig->descriptorPool = createPfxPipelineDescriptorPool(context->device);
    if (pipelineConfig->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    //
    // Create descriptor set layouts
    //
    pipelineConfig->vertexShaderDescriptorSetLayout = createPfxPipelineVertexShaderDescriptorSetLayout(
            context->device);
    if (pipelineConfig->vertexShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader descriptor set layout for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->fragmentShaderDescriptorSetLayout = createPfxPipelineFragmentShaderDescriptorSetLayout(
            context->device);
    if (pipelineConfig->fragmentShaderDescriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader descriptor set layout for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    VkShaderModule vertexShaderModule = createShaderModule(context->device, vertexShaderPath);
    if (vertexShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create vertex shader module for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    VkShaderModule fragmentShaderModule = createShaderModule(context->device, fragmentShaderPath);
    if (fragmentShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create fragment shader module for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    pipelineConfig->pipelineLayout = createPfxPipelineLayout(context->device,
                                                             pipelineConfig->vertexShaderDescriptorSetLayout,
                                                             pipelineConfig->fragmentShaderDescriptorSetLayout);

    if (pipelineConfig->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    Viewport viewport = (Viewport) {
            0, 0,
            (float) vulkanSwapchainContext->swapChainExtent.width,
            (float) vulkanSwapchainContext->swapChainExtent.height,
            0.0f,
            1.0f
    };
    pipelineConfig->pipeline = createPfxPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass,
            viewport,
            vertexShaderModule,
            fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline for pfx shader pipeline");
        destroyPipelineConfig(context, pipelineConfig);
        return NULL;
    }

    vkDestroyShaderModule(context->device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context->device, fragmentShaderModule, NULL);

    return pipelineConfig;
}