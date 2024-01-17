#ifndef VULKAN_COMMANDBUFFER_H
#define VULKAN_COMMANDBUFFER_H

#include "include/entity.h"
#include "include/emitter.h"
#include "include/push_constants.h"
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

void recordDebugCommandBuffer(
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D swapChainExtent,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities,
        VkBuffer unitCubeVertexBuffer,
        Camera *camera);

void recordEmitterBuffer(
        VkCommandBuffer commandBuffer,
        VkFramebuffer framebuffer,
        VkExtent2D swapChainExtent,
        PipelineConfig *pipelineConfig,
        BufferMemory *particleBuffer,
        Emitter *emitter
);
#endif // VULKAN_COMMANDBUFFER_H