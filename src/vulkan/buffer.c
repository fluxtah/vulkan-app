#include "include/vulkan/buffer.h"

void createBufferMemory(VulkanDeviceContext *context, BufferMemory *bufferMemory, VkDeviceSize size,
                        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(context->device, &bufferInfo, NULL, &bufferMemory->buffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create buffer\n");
        exit(1);
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(context->device, bufferMemory->buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(context->physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(context->device, &allocInfo, NULL, &bufferMemory->memory) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate buffer memory\n");
        exit(1);
    }

    if (vkBindBufferMemory(context->device, bufferMemory->buffer, bufferMemory->memory, 0)) {
        fprintf(stderr, "Failed to bind buffer memory\n");
        exit(1);
    }
}

void destroyBufferMemory(VulkanDeviceContext *context, BufferMemory *bufferMemory) {
    vkDestroyBuffer(context->device, bufferMemory->buffer, NULL);
    vkFreeMemory(context->device, bufferMemory->memory, NULL);
}

void createStagedBufferMemory(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        BufferMemory *bufferMemory,
        VkDeviceSize size,
        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void *data) {
    // Create a staging buffer
    BufferMemory stagingBufferMemory;
    stagingBufferMemory = createStagingBufferMemory(context, size, data, &stagingBufferMemory);

    // Create the final buffer with the desired usage
    createBufferMemory(context, bufferMemory, size, usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT, properties);

    // Copy data from staging buffer to the final buffer
    copyBuffer(context->device, commandPool, context->graphicsQueue, stagingBufferMemory.buffer, bufferMemory->buffer,
               size);

    // Cleanup staging buffer
    destroyBufferMemory(context, &stagingBufferMemory);
}

BufferMemory createStagingBufferMemory(VulkanDeviceContext *context, VkDeviceSize size, const void *data,
                                       BufferMemory *stagingBufferMemory) {
    createBufferMemory(context, stagingBufferMemory, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // RenderDataMap the staging buffer and copy data
    void *stagingBufferData;
    if(vkMapMemory(context->device, (*stagingBufferMemory).memory, 0, size, 0, &stagingBufferData) != VK_SUCCESS) {
        fprintf(stderr, "Failed to map staging buffer memory\n");
        exit(1);
    }
    memcpy(stagingBufferData, data, size);
    vkUnmapMemory(context->device, (*stagingBufferMemory).memory);
    return (*stagingBufferMemory);
}

void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size) {
    // Allocate a temporary command buffer for the copy operation
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    if(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to allocate command buffer\n");
        exit(1);
    }

    // Begin recording the command buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        fprintf(stderr, "Failed to begin recording command buffer\n");
        exit(1);
    }

    // Record the copy command
    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // End recording the command buffer
    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        fprintf(stderr, "Failed to record command buffer\n");
        exit(1);
    }

    // Submit the command buffer and wait for it to complete
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    VkFence fence;
    if(vkCreateFence(device, &fenceInfo, NULL, &fence) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create fence\n");
        exit(1);
    }

    if (vkQueueSubmit(queue, 1, &submitInfo, fence) != VK_SUCCESS) {
        fprintf(stderr, "Failed to submit draw command buffer\n");
        exit(1);
    }

    if(vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS) {
        fprintf(stderr, "Failed to wait for fence\n");
        exit(1);
    }

    vkDestroyFence(device, fence, NULL);

    // Free the temporary command buffer
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}
