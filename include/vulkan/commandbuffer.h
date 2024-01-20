#ifndef VULKAN_COMMANDBUFFER_H
#define VULKAN_COMMANDBUFFER_H

#include "include/entity.h"
#include "include/emitter.h"
#include "include/push_constants.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

void beginCommandBufferRecording(
        VkCommandBuffer commandBuffer,
        VkRenderPass renderPass,
        VkFramebuffer framebuffer,
        VkExtent2D *swapChainExtent);

VkCommandBuffer* allocateCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t commandBufferCount);
void recordCommandBuffer(
        ImageMemory *depthImage,
        VkCommandBuffer commandBuffer,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities);

void recordDebugCommandBuffer(
        VkCommandBuffer commandBuffer,
        VkPipeline graphicsPipeline,
        VkPipelineLayout pipelineLayout,
        EntityArray *ktEntities,
        VkBuffer unitCubeVertexBuffer,
        Camera *camera);

void recordEmitterBuffer(
        VkCommandBuffer commandBuffer,
        EmitterArray *emitters
);

void endCommandBufferRecording(VkCommandBuffer commandBuffer);
#endif // VULKAN_COMMANDBUFFER_H