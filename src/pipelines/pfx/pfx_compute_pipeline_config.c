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

void recordComputeCommandBuffer(ComputePipelineConfig *config, Emitter *emitter, float deltaTime) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(config->commandBuffers[0], &beginInfo) != VK_SUCCESS) {
        LOG_ERROR("Failed to begin recording compute command buffer");
        return;
    }

    vkCmdBindPipeline(config->commandBuffers[0], VK_PIPELINE_BIND_POINT_COMPUTE, config->computePipeline);

    vkCmdBindDescriptorSets(config->commandBuffers[0], VK_PIPELINE_BIND_POINT_COMPUTE, config->pipelineLayout, 0, 1, &config->descriptorSet, 0, NULL);

    PfxComputePipelinePushConstants pushConstants = {
            .deltaTime = deltaTime
    };
    glm_mat4_identity(pushConstants.model);

    // Apply rotation and translation first
    glm_translate(pushConstants.model, emitter->position);
    glm_rotate(pushConstants.model, glm_rad(emitter->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(pushConstants.model, glm_rad(emitter->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(pushConstants.model, glm_rad(emitter->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    // Then apply non-uniform scaling
    glm_scale(pushConstants.model, emitter->scale);

    vkCmdPushConstants(config->commandBuffers[0], config->pipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(PfxComputePipelinePushConstants), &pushConstants);

    uint32_t workGroupSize = 32; // Example workgroup size
    uint32_t dispatchCount = (MAX_PARTICLE_COUNT + workGroupSize - 1) / workGroupSize;
    vkCmdDispatch(config->commandBuffers[0], dispatchCount, 1, 1);

    // Memory Barrier
    VkBufferMemoryBarrier bufferBarrier = {};
    bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    bufferBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    bufferBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
    bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    bufferBarrier.buffer = config->particleBuffer->buffer;
    bufferBarrier.offset = 0;
    bufferBarrier.size = VK_WHOLE_SIZE;

    vkCmdPipelineBarrier(
            config->commandBuffers[0],
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
            0, 0, NULL, 1, &bufferBarrier, 0, NULL);

    if (vkEndCommandBuffer(config->commandBuffers[0]) != VK_SUCCESS) {
        LOG_ERROR("Failed to record compute command buffer");
    }
}
