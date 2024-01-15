#include "include/pipelines/basic/basic_pipeline_frag_descriptor_set_layout.h"

VkDescriptorSetLayout createBasicPipelineFragmentShaderDescriptorSetLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding lightingUboLayoutBinding = {};
    lightingUboLayoutBinding.binding = 0;
    lightingUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lightingUboLayoutBinding.descriptorCount = 1;
    lightingUboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    lightingUboLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutBinding normalMapLayoutBinding = {};
    normalMapLayoutBinding.binding = 2;
    normalMapLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    normalMapLayoutBinding.descriptorCount = 1;
    normalMapLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    normalMapLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutBinding metallicRoughnessMapLayoutBinding = {};
    metallicRoughnessMapLayoutBinding.binding = 3;
    metallicRoughnessMapLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    metallicRoughnessMapLayoutBinding.descriptorCount = 1;
    metallicRoughnessMapLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    metallicRoughnessMapLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutBinding bindings[] = {
            lightingUboLayoutBinding,
            samplerLayoutBinding,
            normalMapLayoutBinding,
            metallicRoughnessMapLayoutBinding
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 4;
    layoutInfo.pBindings = bindings;

    VkDescriptorSetLayout descriptorSetLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create fragment descriptor set layout");
        return VK_NULL_HANDLE;
    }

    return descriptorSetLayout;
}