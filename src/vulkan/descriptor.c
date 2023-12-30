#include "include/vulkan/descriptor.h"

void createBasicShaderDescriptorPool(VkDevice device, VkDescriptorPool *descriptorPool) {
    VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         9},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 12} // diffuse and normal map
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 6; // Total number of descriptor sets

    if (vkCreateDescriptorPool(device, &poolInfo, NULL, descriptorPool) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create descriptor pool\n");
        exit(-1);
    }
}

VkDescriptorSetLayout createVertexShaderDescriptorSetLayout(VkDevice device) {
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
        fprintf(stderr, "Failed to create vertex descriptor set layout\n");
        exit(-1);
    }

    return descriptorSetLayout;
}

VkDescriptorSetLayout createFragmentShaderDescriptorSetLayout(VkDevice device) {
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
        fprintf(stderr, "Failed to create fragment descriptor set layout\n");
        exit(-1);
    }

    return descriptorSetLayout;
}

void allocateDescriptorSet(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkDescriptorSet *descriptorSet) {

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSet) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate descriptor set\n");
        exit(-1);
    }
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
