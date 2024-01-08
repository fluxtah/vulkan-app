#include "include/vulkan/commandbuffer.h"

void beginCommandBufferRecording(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer,
                                 VkExtent2D *swapChainExtent, VkPipeline graphicsPipeline);

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
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D swapChainExtent,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities) {
    beginCommandBufferRecording(commandBuffer, renderPass, framebuffer, &swapChainExtent, graphicsPipeline);

    for (size_t i = 0; i < ktEntities->size; i++) {
        RenderObject *obj = (RenderObject *) (ktEntities->entities[i]);

        VkBuffer vertexBuffers[] = {obj->vertexBuffer->buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, obj->indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT16);

        VkDescriptorSet descriptorSets[] = { obj->vertexDescriptorSet, obj->fragmentDescriptorSet };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 2, descriptorSets, 0, NULL);

        vkCmdDrawIndexed(commandBuffer, obj->modelData->num_indices, 1, 0, 0, 0);
    }
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to record command buffer\n");
        exit(-1);
    }
}

void beginCommandBufferRecording(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer,
                                 VkExtent2D *swapChainExtent, VkPipeline graphicsPipeline) {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;               // Optional
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

    VkClearValue clearValues[2];
    clearValues[0].color = (VkClearColorValue) {{0.0f, 0.0f, 0.0f, 1.0f}}; // Clear color
    clearValues[1].depthStencil = (VkClearDepthStencilValue) {1.0f, 0};    // Clear depth

    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}
