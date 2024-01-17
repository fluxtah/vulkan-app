#include "include/pipelines/pfx/pfx_pipeline_descriptor_pool.h"

VkDescriptorPool createPfxPipelineDescriptorPool(VkDevice device) {
    VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         50}, // TODO gonna need to get a real number here
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 50} // maps
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 100; // Total number of descriptor sets

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptorPool) != VK_SUCCESS) {
        LOG_ERROR("Failed to create descriptor pool");
        return VK_NULL_HANDLE;
    }
    return descriptorPool;
}
