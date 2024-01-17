#include "include/pipelines/pfx/pfx_pipeline_vertex_descriptor_set_layout.h"

VkDescriptorSetLayout createPfxPipelineVertexShaderDescriptorSetLayout(VkDevice device) {
    VkDescriptorSetLayoutBinding transformUboLayoutBinding = {};
    transformUboLayoutBinding.binding = 0;
    transformUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    transformUboLayoutBinding.descriptorCount = 1;
    transformUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    transformUboLayoutBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &transformUboLayoutBinding;

    VkDescriptorSetLayout descriptorSetLayout;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS) {
        LOG_ERROR("Failed to create vertex descriptor set layout");
        return VK_NULL_HANDLE;
    }

    return descriptorSetLayout;
}