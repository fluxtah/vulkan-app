#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "include/context.h"
#include "include/vulkan/memory.h"
#include "buffer_memory.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct VulkanDeviceContext VulkanDeviceContext;

void createBufferMemory(VulkanDeviceContext *context, BufferMemory *bufferMemory, VkDeviceSize size,
                        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

void destroyBufferMemory(VulkanDeviceContext *context, BufferMemory *bufferMemory);

BufferMemory createStagingBufferMemory(VulkanDeviceContext *context, VkDeviceSize size, const void *data,
                                       BufferMemory *stagingBufferMemory);

void createStagedBufferMemory(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        BufferMemory *bufferMemory,
        VkDeviceSize size,
        VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void *data);

void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size);

#endif // VULKAN_BUFFER_H