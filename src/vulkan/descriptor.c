#include "include/vulkan/descriptor.h"

VkResult allocateDescriptorSet(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkDescriptorSet *descriptorSet) {

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    return vkAllocateDescriptorSets(device, &allocInfo, descriptorSet);
}

void updateBasicShaderDescriptorSet(
        VkDevice device,
        VkDescriptorSet vertexDescriptorSet,
        VkDescriptorSet fragmentDescriptorSet,
        VkBuffer transformUboBuffer,
        VkBuffer lightingUboBuffer,
        VkImageView baseColorImageView,
        VkImageView normalMapImageView,
        VkImageView metallicRoughnessMapImageView,
        VkSampler sampler) {
    VkDescriptorBufferInfo transformUboBufferInfo = {};
    transformUboBufferInfo.buffer = transformUboBuffer;
    transformUboBufferInfo.offset = 0;
    transformUboBufferInfo.range = sizeof(TransformUBO);

    VkDescriptorBufferInfo lightingUboBufferInfo = {};
    lightingUboBufferInfo.buffer = lightingUboBuffer;
    lightingUboBufferInfo.offset = 0;
    lightingUboBufferInfo.range = sizeof(LightingUBO);

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
    descriptorWrite.dstSet = vertexDescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &transformUboBufferInfo;

    VkWriteDescriptorSet descriptorWrite1 = {};
    descriptorWrite1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite1.dstSet = fragmentDescriptorSet;
    descriptorWrite1.dstBinding = 0;
    descriptorWrite1.dstArrayElement = 0;
    descriptorWrite1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite1.descriptorCount = 1;
    descriptorWrite1.pBufferInfo = &lightingUboBufferInfo;

    VkWriteDescriptorSet descriptorWrite2 = {};
    descriptorWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite2.dstSet = fragmentDescriptorSet;
    descriptorWrite2.dstBinding = 1;
    descriptorWrite2.dstArrayElement = 0;
    descriptorWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite2.descriptorCount = 1;
    descriptorWrite2.pBufferInfo = NULL;
    descriptorWrite2.pImageInfo = &textureImageInfo;

    VkWriteDescriptorSet descriptorWrite3 = {};
    descriptorWrite3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite3.dstSet = fragmentDescriptorSet;
    descriptorWrite3.dstBinding = 2;
    descriptorWrite3.dstArrayElement = 0;
    descriptorWrite3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite3.descriptorCount = 1;
    descriptorWrite3.pBufferInfo = NULL;
    descriptorWrite3.pImageInfo = &normalMapImageInfo;

    VkWriteDescriptorSet descriptorWrite4 = {};
    descriptorWrite4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite4.dstSet = fragmentDescriptorSet;
    descriptorWrite4.dstBinding = 3;
    descriptorWrite4.dstArrayElement = 0;
    descriptorWrite4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite4.descriptorCount = 1;
    descriptorWrite4.pBufferInfo = NULL;
    descriptorWrite4.pImageInfo = &metallicRoughnessMapImageInfo;

    VkWriteDescriptorSet sets[] = {
            descriptorWrite,
            descriptorWrite1,
            descriptorWrite2,
            descriptorWrite3,
            descriptorWrite4
    };

    vkUpdateDescriptorSets(device, 5, sets, 0, NULL);
}
