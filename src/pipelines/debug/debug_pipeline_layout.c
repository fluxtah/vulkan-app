#include "include/pipelines/debug/debug_pipeline_layout.h"

VkPipelineLayout createDebugPipelineLayout(VkDevice device) {

    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Accessible in the vertex shader
    pushConstantRange.offset = 0;
    pushConstantRange.size = 192; // Size for model, view, proj matrices

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = NULL;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;


    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create basic pipeline layout");
        return VK_NULL_HANDLE;
    }

    return pipelineLayout;
}