#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "include/vulkan/memory.h"
#include "context.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    VkBuffer buffer;
    VkDeviceMemory memory;
} BufferMemory;

void createBufferMemory(VulkanContext *context, BufferMemory *bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage,
                        VkMemoryPropertyFlags properties);

void destroyBufferMemory(VulkanContext *context, BufferMemory *bufferMemory);

BufferMemory createStagingBufferMemory(VulkanContext *context, VkDeviceSize size, const void *data, BufferMemory *stagingBufferMemory);

void createStagedBufferMemory(VulkanContext *context, VkCommandPool commandPool,
                              BufferMemory *bufferMemory, VkDeviceSize size,
                              VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void *data);

void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size);

#endif // VULKAN_BUFFER_H