#include "include/pipelines/pfx/pfx_compute_pipeline_layout.h"

VkPipelineLayout createPfxComputePipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout) {
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT; // Specify the shader stage this push constant is for
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(float); // Size of deltaTime

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1; // No descriptor sets in this example
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout; // No descriptor sets
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, NULL, &pipelineLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create pipeline layout");
        return VK_NULL_HANDLE;
    }

    return pipelineLayout;
}