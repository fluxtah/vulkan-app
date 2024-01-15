#include "include/pipelines/pipeline_shader_util.h"

VkPipelineShaderStageCreateInfo *createBasicShaderStages(
        VkShaderModule vertShaderModule, VkShaderModule fragShaderModule) {
    VkPipelineShaderStageCreateInfo *shaderStages = malloc(sizeof(VkPipelineShaderStageCreateInfo) * 2);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // Vertex shader
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main"; // Entry point in the shader

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; // Fragment shader
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main"; // Entry point in the shader

    shaderStages[0] = vertShaderStageInfo;
    shaderStages[1] = fragShaderStageInfo;

    return shaderStages;
}