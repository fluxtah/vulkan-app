#include "include/pipelines/pfx/pfx_compute_pipeline.h"

VkPipeline createPfxComputePipeline(VkDevice device, VkPipelineLayout layout, const char *shaderPath) {
    VkShaderModule computeShaderModule = createShaderModule(device, shaderPath);
    if (computeShaderModule == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create compute shader module");
        return VK_NULL_HANDLE;
    }

    VkPipelineShaderStageCreateInfo computeShaderStageInfo = {};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;
    computeShaderStageInfo.pName = "main";

    VkComputePipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stage = computeShaderStageInfo;
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