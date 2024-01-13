#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "imagememory.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/camera.h"
#include "sound.h"
#include "include/vulkan/buffer_memory.h"

typedef struct ImageMemory ImageMemory;

typedef struct PipelineConfig {
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout vertexShaderDescriptorSetLayout;
    VkDescriptorSetLayout fragmentShaderDescriptorSetLayout;
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    VkCommandBuffer *commandBuffers;
    VkRenderPass renderPass;
    VkFramebuffer *swapChainFramebuffers;
} PipelineConfig;

typedef struct VulkanDeviceContext {
    GLFWwindow *window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkFormat depthFormat;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
} VulkanDeviceContext;

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
    BufferMemory* debugCubeBuffer;
    bool debugBoundingVolumes;
} ApplicationContext;

#endif //VULKAN_CONTEXT_H
