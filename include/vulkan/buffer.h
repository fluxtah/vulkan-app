#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "include/vulkan/memory.h"
#include "include/context.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    VkBuffer buffer;
    VkDeviceMemory memory;
} BufferMemory;

void createBufferMemory(ApplicationContext *context, BufferMemory *bufferMemory, VkDeviceSize size, VkBufferUsageFlags usage,
                        VkMemoryPropertyFlags properties);

void destroyBufferMemory(ApplicationContext *context, BufferMemory *bufferMemory);

BufferMemory createStagingBufferMemory(ApplicationContext *context, VkDeviceSize size, const void *data, BufferMemory *stagingBufferMemory);

void createStagedBufferMemory(ApplicationContext *context, VkCommandPool commandPool,
                              BufferMemory *bufferMemory, VkDeviceSize size,
                              VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, void *data);

void copyBuffer(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size);

#endif // VULKAN_BUFFER_H