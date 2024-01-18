#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "imagememory.h"
#include <stdio.h>
#include <stdlib.h>
#include "include/camera.h"
#include "sound.h"
#include "include/vulkan/buffer_memory.h"
#include "include/pipelines/pipeline_config.h"

typedef struct ImageMemory ImageMemory;

typedef struct VulkanSwapchainContext {
    // TODO Screen dependent resources properties
    //  when the screen changes or device reset (gotta research this)
    //  may need to recreate these resources
    VkExtent2D swapChainExtent;
    VkSwapchainKHR swapChain;
    VkImageView *swapChainImageViews;
    uint32_t swapChainImageCount;
    ImageMemory *depthStencil;
} VulkanSwapchainContext;

typedef struct ApplicationContext {
    VulkanDeviceContext *vulkanDeviceContext;
    VulkanSwapchainContext *vulkanSwapchainContext;
    VkCommandPool commandPool;
    VkSampler sampler;
    PipelineConfig *pipelineConfig;
    PipelineConfig *debugPipelineConfig;
    AudioContext *audioContext;
    Camera *activeCamera;
    BufferMemory *debugCubeBuffer;
    bool debugBoundingVolumes;
    VkRenderPass renderPass;
    VkFramebuffer *swapChainFramebuffers;
    VkCommandBuffer *commandBuffers;
} ApplicationContext;

#endif //VULKAN_CONTEXT_H
