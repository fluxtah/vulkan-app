#include "include/vulkan/commandbuffer.h"
#include "include/pipelines/pfx/particle.h"

VkCommandBuffer *allocateCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount) {
    VkCommandBuffer *commandBuffers = malloc(sizeof(VkCommandBuffer) * commandBufferCount);
    if (commandBuffers == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to allocate memory for command buffers\n");
        return VK_NULL_HANDLE;
    }

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = commandBufferCount;

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate command buffers\n");
        free(commandBuffers);
        return VK_NULL_HANDLE;
    }

    return commandBuffers;
}

void recordCommandBuffer(
        ImageMemory *depthImage,
        VkCommandBuffer commandBuffer,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    for (size_t i = 0; i < ktEntities->size; i++) {
        Entity *obj = (Entity *) (ktEntities->entities[i]);

        VkBuffer vertexBuffers[] = {obj->renderResources->vertexBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, obj->renderResources->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        VkDescriptorSet descriptorSets[] = {obj->vertexDescriptorSet, obj->fragmentDescriptorSet};
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, descriptorSets, 0,
                                NULL);

        vkCmdDrawIndexed(commandBuffer, obj->renderResources->modelData->num_indices, 1, 0, 0, 0);
    }

//    VkImageMemoryBarrier barrier = {};
//    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//    barrier.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//    barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//    barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//    barrier.image = depthImage->image; // The depth buffer image
//    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
//    barrier.subresourceRange.baseMipLevel = 0;
//    barrier.subresourceRange.levelCount = 1;
//    barrier.subresourceRange.baseArrayLayer = 0;
//    barrier.subresourceRange.layerCount = 1;
//
//    vkCmdPipelineBarrier(
//            commandBuffer,
//            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, // or VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
//            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, // for the second render pass
//            0,
//            0, NULL,
//            0, NULL,
//            1, &barrier
//    );
}

void beginCommandBufferRecording(
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D *swapChainExtent) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;               // Optional
    beginInfo.pInheritanceInfo = NULL; // Optional for primary command buffers

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        fprintf(stderr, "Failed to begin recording command buffer\n");
        exit(-1);
    }

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = (VkOffset2D) {0, 0};
    renderPassInfo.renderArea.extent = (*swapChainExtent);

    // Clear depth at the start of the render pass
    VkClearValue clearValues[2];
    clearValues[0].color = (VkClearColorValue) {{0.0f, 0.0f, 0.0f, 1.0f}}; // Clear color
    clearValues[1].depthStencil = (VkClearDepthStencilValue) {1.0f, 0};    // Clear depth

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues;
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void calculateAABBTransform(Entity *entity, mat4 transform) {
    glm_mat4_identity(transform);

    // Calculate the center of the AABB
    vec3 center;
    glm_vec3_add(entity->aabb.min, entity->aabb.max, center);
    glm_vec3_scale(center, 0.5f, center);

    // Calculate the scale needed to match the size of the AABB
    vec3 scale;
    glm_vec3_sub(entity->aabb.max, entity->aabb.min, scale);

    // Apply transformations
    glm_translate(transform, center); // Move to the center of the AABB
    glm_scale(transform, scale); // Scale to match the AABB size
}

void calculateOBBTransform(Entity *entity, mat4 transform) {
    // Start with the OBB's transformation matrix
    memcpy(transform, entity->obb.transform, sizeof(mat4));

    // Scale the unit cube to match the full dimensions of the AABB
    vec3 fullExtents;
    glm_vec3_scale(entity->obb.extents, 2.0f, fullExtents); // Scale the extents by 2
    glm_scale(transform, fullExtents);
}

void recordDebugCommandBuffer(
        VkCommandBuffer commandBuffer,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities,
        VkBuffer unitCubeVertexBuffer,
        Camera *camera) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    for (size_t i = 0; i < ktEntities->size; ++i) {
        Entity *entity = (Entity *) (ktEntities->entities[i]);
        mat4 transform;

        if (entity->useOBB)
            calculateOBBTransform(entity, transform);
        else
            calculateAABBTransform(entity, transform);

        PushConstants pushConstants = {0};
        memcpy(pushConstants.view, camera->view, sizeof(mat4));
        memcpy(pushConstants.proj, camera->proj, sizeof(mat4));
        memcpy(pushConstants.model, transform, sizeof(mat4));

        // Push the transform to the shader
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants),
                           &pushConstants);

        VkBuffer buffers[] = {unitCubeVertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        // Assuming you have a predefined way to get the number of vertices for your unit cube
        vkCmdDraw(commandBuffer, 24, 1, 0, 0);
    }
}

void recordEmitterBuffer(
        VkCommandBuffer commandBuffer,
        PipelineConfig *pipelineConfig,
        BufferMemory *particleBuffer,
        Emitter *emitter
) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineConfig->pipeline);

    VkBuffer vertexBuffers[] = {
            emitter->renderResources->vertexBuffer->buffer,
            particleBuffer->buffer
    };
    VkDeviceSize offsets[] = {0, 0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, emitter->renderResources->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

    VkDescriptorSet descriptorSets[] = {emitter->vertexDescriptorSet, emitter->fragmentDescriptorSet};
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineConfig->pipelineLayout, 0, 2,
                            descriptorSets, 0,
                            NULL);

    vkCmdDrawIndexed(commandBuffer, emitter->renderResources->modelData->num_indices, MAX_PARTICLE_COUNT, 0, 0, 0);
}

void endCommandBufferRecording(VkCommandBuffer commandBuffer) {
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to record emitter command buffer\n");
        exit(-1);
    }
}
