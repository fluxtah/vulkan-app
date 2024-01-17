#include "include/pipelines/pfx/pfx_compute_pipeline_config.h"
#include "include/pipelines/pfx/particle.h"

ComputePipelineConfig *createPfxComputePipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool) {
    ComputePipelineConfig *config = malloc(sizeof(ComputePipelineConfig));

    config->descriptorPool = createPfxComputePipelineDescriptorPool(context->device);
    if (config->descriptorPool == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor pool for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->descriptorSetLayout = createPfxComputePipelineDescriptorSetLayout(context->device);
    if (config->descriptorSetLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create descriptor set layout for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->pipelineLayout = createPfxComputePipelineLayout(context->device, config->descriptorSetLayout);
    if (config->pipelineLayout == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pipeline layout for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->computePipeline = createPfxComputePipeline(context->device, config->pipelineLayout);
    if (config->computePipeline == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to create pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    config->commandBuffers = allocateCommandBuffers(context->device, commandPool, 1);
    if (config->commandBuffers == VK_NULL_HANDLE) {
        LOG_ERROR("Failed to allocate command buffers for basic shader pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
        return NULL;
    }

    VkDeviceSize bufferSize = sizeof(Particle) * MAX_PARTICLE_COUNT;

    config->particleBuffer = (BufferMemory *) malloc(sizeof(BufferMemory));

    createBufferMemory(context, config->particleBuffer, bufferSize,
                       VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

     if(allocateDescriptorSet(
            context->device,
            config->descriptorPool,
            config->descriptorSetLayout,
            &config->descriptorSet) != VK_SUCCESS) {
        LOG_ERROR("Failed to allocate descriptor set for pfx compute pipeline");
        destroyComputePipelineConfig(
                context,
                commandPool,
                config);
     }

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = config->particleBuffer->buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(Particle) * MAX_PARTICLE_COUNT;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = config->descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(context->device, 1, &descriptorWrite, 0, NULL);

    return config;
}

void recordComputeCommandBuffer(ComputePipelineConfig *config, float deltaTime) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(config->commandBuffers[0], &beginInfo) != VK_SUCCESS) {
        LOG_ERROR("Failed to begin recording compute command buffer");
        return;
    }

    vkCmdBindPipeline(config->commandBuffers[0], VK_PIPELINE_BIND_POINT_COMPUTE, config->computePipeline);

    vkCmdBindDescriptorSets(config->commandBuffers[0], VK_PIPELINE_BIND_POINT_COMPUTE, config->pipelineLayout, 0, 1, &config->descriptorSet, 0, NULL);

    vkCmdPushConstants(config->commandBuffers[0], config->pipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(float), &deltaTime);

    vkCmdDispatch(config->commandBuffers[0], MAX_PARTICLE_COUNT / 32, 1, 1);

    if (vkEndCommandBuffer(config->commandBuffers[0]) != VK_SUCCESS) {
        LOG_ERROR("Failed to record compute command buffer");
    }
}
