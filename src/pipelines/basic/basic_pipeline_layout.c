#include "include/pipelines/basic/basic_pipeline_layout.h"

VkPipelineLayout createBasicPipelineLayout(
        VkDevice device,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout) {

    VkDescriptorSetLayout layouts[] = {
            vertexDescriptorSetLayout,
            fragmentDescriptorSetLayout
    };

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 2; // We have two descriptor set layouts
    pipelineLayoutInfo.pSetLayouts = layouts; // Pointer to descriptor set layout
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = NULL; // Optional


    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create basic pipeline layout");
        return VK_NULL_HANDLE;
    }

    return pipelineLayout;
}