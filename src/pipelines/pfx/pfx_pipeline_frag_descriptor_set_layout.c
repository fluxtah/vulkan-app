#include "include/pipelines/pfx/pfx_pipeline_frag_descriptor_set_layout.h"

VkDescriptorSetLayout createPfxPipelineFragmentShaderDescriptorSetLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutBinding bindings[] = {
            samplerLayoutBinding,
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = bindings;

    VkDescriptorSetLayout descriptorSetLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create fragment descriptor set layout");
        return VK_NULL_HANDLE;
    }

    return descriptorSetLayout;
}