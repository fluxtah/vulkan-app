#include "include/pipelineconfig.h"

PipelineConfig *createBasicShaderPipelineConfig(VulkanDeviceContext *context, VkRenderPass renderPass, VkExtent2D swapChainExtent) {
    PipelineConfig  *pipelineConfig = malloc(sizeof(PipelineConfig));

    //
    // Create a descriptor pool
    //
    pipelineConfig->descriptorPool = createBasicShaderDescriptorPool(context->device);

    //
    // Create descriptor set layouts
    //
    pipelineConfig->vertexShaderDescriptorSetLayout = createVertexShaderDescriptorSetLayout(context->device);
    pipelineConfig->fragmentShaderDescriptorSetLayout = createFragmentShaderDescriptorSetLayout(context->device);


    pipelineConfig->vertexShaderModule = createShaderModule(context->device, "shaders/basic.vert.spv");
    pipelineConfig->fragmentShaderModule = createShaderModule(context->device, "shaders/basic.frag.spv");

    pipelineConfig->pipelineLayout = createPipelineLayout(context->device,
                                                          pipelineConfig->vertexShaderDescriptorSetLayout,
                                                          pipelineConfig->fragmentShaderDescriptorSetLayout);
    Viewport viewport = (Viewport) {
            0, 0,
            (float) swapChainExtent.width,
            (float) swapChainExtent.height,
            0.0f,
            1.0f
    };
    pipelineConfig->pipeline = createPipeline(
            context->device,
            pipelineConfig->pipelineLayout,
            renderPass, viewport,
            pipelineConfig->vertexShaderModule,
            pipelineConfig->fragmentShaderModule);

    if (pipelineConfig->pipeline == VK_NULL_HANDLE)
        return NULL;

    return pipelineConfig;
}

void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig) {
    vkDestroyDescriptorPool(context->device, pipelineConfig->descriptorPool, NULL);
    vkDestroyDescriptorSetLayout(context->device, pipelineConfig->vertexShaderDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(context->device, pipelineConfig->fragmentShaderDescriptorSetLayout, NULL);
    vkDestroyPipeline(context->device, pipelineConfig->pipeline, NULL);
    vkDestroyPipelineLayout(context->device, pipelineConfig->pipelineLayout, NULL);
    vkDestroyShaderModule(context->device, pipelineConfig->vertexShaderModule, NULL);
    vkDestroyShaderModule(context->device, pipelineConfig->fragmentShaderModule, NULL);
    free(pipelineConfig);
}