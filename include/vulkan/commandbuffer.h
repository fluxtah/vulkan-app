#ifndef VULKAN_COMMANDBUFFER_H
#define VULKAN_COMMANDBUFFER_H

#include "include/renderobject.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkCommandBuffer* allocateCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount);
void recordCommandBuffer(
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D swapChainExtent,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities);
#endif // VULKAN_COMMANDBUFFER_H