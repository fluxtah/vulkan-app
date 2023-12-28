#include "include/vulkan/descriptor.h"

void createBasicShaderDescriptorPool(VkDevice device, VkDescriptorPool *descriptorPool) {
    VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         3},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 9} // diffuse and normal map
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 3; // Total number of descriptor sets

    if (vkCreateDescriptorPool(device, &poolInfo, NULL, descriptorPool) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create descriptor pool\n");
        exit(-1);
    }
}

void createBasicShaderDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout *descriptorSetLayout) {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    uboLayoutBinding.pImmutableSamplers = NULL;

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
            uboLayoutBinding,
            samplerLayoutBinding,
            normalMapLayoutBinding,
            metallicRoughnessMapLayoutBinding
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 4;
    layoutInfo.pBindings = bindings;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, descriptorSetLayout) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create descriptor set layout\n");
        exit(-1);
    }
}

void allocateBasicShaderDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool,
                                      VkDescriptorSetLayout descriptorSetLayout,
                                      VkDescriptorSet *descriptorSet) {
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSet) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate descriptor set\n");
        exit(-1);
    }
}

void updateBasicShaderDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet, VkBuffer buffer,
                                    VkImageView baseColorImageView, VkImageView normalMapImageView, VkImageView metallicRoughnessMapImageView, VkSampler sampler) {
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorImageInfo textureImageInfo = {};
    textureImageInfo.imageView = baseColorImageView;
    textureImageInfo.sampler = sampler;
    textureImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorImageInfo normalMapImageInfo = {};
    normalMapImageInfo.imageView = normalMapImageView;
    normalMapImageInfo.sampler = sampler;
    normalMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkDescriptorImageInfo metallicRoughnessMapImageInfo = {};
    metallicRoughnessMapImageInfo.imageView = metallicRoughnessMapImageView;
    metallicRoughnessMapImageInfo.sampler = sampler;
    metallicRoughnessMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    VkWriteDescriptorSet descriptorWrite2 = {};
    descriptorWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite2.dstSet = descriptorSet;
    descriptorWrite2.dstBinding = 1;
    descriptorWrite2.dstArrayElement = 0;
    descriptorWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite2.descriptorCount = 1;
    descriptorWrite2.pBufferInfo = NULL;
    descriptorWrite2.pImageInfo = &textureImageInfo;

    VkWriteDescriptorSet descriptorWrite3 = {};
    descriptorWrite3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite3.dstSet = descriptorSet;
    descriptorWrite3.dstBinding = 2;
    descriptorWrite3.dstArrayElement = 0;
    descriptorWrite3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite3.descriptorCount = 1;
    descriptorWrite3.pBufferInfo = NULL;
    descriptorWrite3.pImageInfo = &normalMapImageInfo;

    VkWriteDescriptorSet descriptorWrite4 = {};
    descriptorWrite4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite4.dstSet = descriptorSet;
    descriptorWrite4.dstBinding = 3;
    descriptorWrite4.dstArrayElement = 0;
    descriptorWrite4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite4.descriptorCount = 1;
    descriptorWrite4.pBufferInfo = NULL;
    descriptorWrite4.pImageInfo = &metallicRoughnessMapImageInfo;

    VkWriteDescriptorSet sets[] = {
            descriptorWrite,
            descriptorWrite2,
            descriptorWrite3,
            descriptorWrite4
    };

    vkUpdateDescriptorSets(device, 4, sets, 0, NULL);
}
