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

typedef struct ImageMemory ImageMemory;

typedef struct PipelineConfig {
    VkShaderModule vertexShaderModule;
    VkShaderModule fragmentShaderModule;
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout vertexShaderDescriptorSetLayout;
    VkDescriptorSetLayout fragmentShaderDescriptorSetLayout;
    VkDescriptorPool descriptorPool;
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
    VkFramebuffer *swapChainFramebuffers;
    ImageMemory *depthStencil;
    VkRenderPass renderPass;
} VulkanSwapchainContext;

typedef struct ApplicationContext {
    VulkanDeviceContext *vulkanDeviceContext;
    VulkanSwapchainContext *vulkanSwapchainContext;
    VkCommandPool commandPool;
    VkSampler sampler;
    VkCommandBuffer *commandBuffers;
    PipelineConfig *pipelineConfig;
    AudioContext *audioContext;
    Camera *activeCamera;
} ApplicationContext;

#endif //VULKAN_CONTEXT_H
