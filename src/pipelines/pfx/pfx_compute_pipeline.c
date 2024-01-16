#include "include/pipelines/pfx/pfx_compute_pipeline.h"

VkPipeline createPfxComputePipeline(VkDevice device, VkPipelineLayout layout) {
    VkShaderModule computeShaderModule = createShaderModule(device, "shaders/particle.comp.spv");
    if (computeShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create compute shader module");
        return VK_NULL_HANDLE;
    }

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // Vertex shader
    vertShaderStageInfo.module = computeShaderModule;
    vertShaderStageInfo.pName = "main"; // Entry point in the shader

    VkComputePipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stage = vertShaderStageInfo;
    pipelineCreateInfo.layout = layout;

    VkPipeline computePipeline;
    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &computePipeline) !=
        VK_SUCCESS) {
        vkDestroyShaderModule(device, computeShaderModule, NULL);
        LOG_ERROR("Failed to create compute pipeline");
        return VK_NULL_HANDLE;
    }

    vkDestroyShaderModule(device, computeShaderModule, NULL);

    return computePipeline;
}