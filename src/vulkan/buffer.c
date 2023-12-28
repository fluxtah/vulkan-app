#include "include/vulkan/buffer.h"

void createBufferMemory(VulkanContext *context, BufferMemory *bufferMemory, VkDeviceSize size,
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

    vkBindBufferMemory(context->device, bufferMemory->buffer, bufferMemory->memory, 0);
}

void destroyBufferMemory(VulkanContext *context, BufferMemory *bufferMemory) {
    vkDestroyBuffer(context->device, bufferMemory->buffer, NULL);
    vkFreeMemory(context->device, bufferMemory->memory, NULL);
}

void createStagedBufferMemory(
        VulkanContext *context,
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
    copyBuffer(context->device, commandPool, context->graphicsQueue, stagingBufferMemory.buffer, bufferMemory->buffer, size);

    // Cleanup staging buffer
    destroyBufferMemory(context, &stagingBufferMemory);
}

BufferMemory createStagingBufferMemory(VulkanContext *context, VkDeviceSize size, const void *data, BufferMemory *stagingBufferMemory) {
    createBufferMemory(context, stagingBufferMemory, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // Map the staging buffer and copy data
    void *stagingBufferData;
    vkMapMemory(context->device, (*stagingBufferMemory).memory, 0, size, 0, &stagingBufferData);
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
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    // Begin recording the command buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    // Record the copy command
    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    // End recording the command buffer
    vkEndCommandBuffer(commandBuffer);

    // Submit the command buffer and wait for it to complete
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);

    // Free the temporary command buffer
    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}
