#include "include/pipelines/pfx/pfx_pipeline_update_descriptor_sets.h"

void updatePfxPipelineDescriptorSets(
        VkDevice device,
        VkDescriptorSet vertexDescriptorSet,
        VkDescriptorSet fragmentDescriptorSet,
        VkBuffer transformUboBuffer,
        VkImageView baseColorImageView,
        VkSampler sampler) {
    VkDescriptorBufferInfo transformUboBufferInfo = {};
    transformUboBufferInfo.buffer = transformUboBuffer;
    transformUboBufferInfo.offset = 0;
    transformUboBufferInfo.range = sizeof(TransformUBO);

    VkDescriptorImageInfo textureImageInfo = {};
    textureImageInfo.imageView = baseColorImageView;
    textureImageInfo.sampler = sampler;
    textureImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

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
    descriptorWrite1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite1.descriptorCount = 1;
    descriptorWrite1.pBufferInfo = NULL;
    descriptorWrite1.pImageInfo = &textureImageInfo;

    VkWriteDescriptorSet sets[] = {
            descriptorWrite,
            descriptorWrite1,
    };

    vkUpdateDescriptorSets(device, 2, sets, 0, NULL);
}
