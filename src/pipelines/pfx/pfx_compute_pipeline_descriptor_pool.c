#include "include/pipelines/pfx/pfx_compute_pipeline_descriptor_pool.h"

VkDescriptorPool createPfxComputePipelineDescriptorPool(VkDevice device) {
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.poolSizeCount = 1;
    poolCreateInfo.pPoolSizes = &poolSize;
    poolCreateInfo.maxSets = 1;

    VkDescriptorPool descriptorPool;
    if (vkCreateDescriptorPool(device, &poolCreateInfo, NULL, &descriptorPool) != VK_SUCCESS) {
        LOG_ERROR("Failed to create descriptor pool");
        return VK_NULL_HANDLE;
    }

    return descriptorPool;
}