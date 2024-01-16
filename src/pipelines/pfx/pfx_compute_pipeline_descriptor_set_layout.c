#include "include/pipelines/pfx/pfx_compute_pipeline_descriptor_set_layout.h"

VkDescriptorSetLayout createPfxComputePipelineDescriptorSetLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding ssboLayoutBinding = {};
    ssboLayoutBinding.binding = 0;
    ssboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    ssboLayoutBinding.descriptorCount = 1;
    ssboLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT; // Used in compute shader

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = 1;
    layoutCreateInfo.pBindings = &ssboLayoutBinding;

    VkDescriptorSetLayout descriptorSetLayout;

    if (vkCreateDescriptorSetLayout(device, &layoutCreateInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create descriptor set layout");
        return VK_NULL_HANDLE;
    }

    return descriptorSetLayout;
}

