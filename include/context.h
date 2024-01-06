//
// Created by Ian Warwick on 24/12/2023.
//

#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/camera.h"
#include "sound.h"

typedef struct ApplicationContext {
    //
    // Vulkan & GLFW
    //
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
    VkCommandPool commandPool;
    VkSampler sampler;

    // TODO Screen dependent data properties
    //  when the screen changes or device reset (gotta research this)
    //  may need to recreate these resources
    VkExtent2D swapChainExtent;
    VkSwapchainKHR swapChain;
    VkImageView *swapChainImageViews;
    uint32_t swapChainImageCount;
    VkFramebuffer *swapChainFramebuffers;

    VkDescriptorSetLayout vertexShaderDescriptorSetLayout;
    VkDescriptorSetLayout fragmentShaderDescriptorSetLayout;
    VkDescriptorPool descriptorPool;

    //
    // Audio
    //
    AudioContext *audioContext;

    //
    // App Data
    //
    Camera *activeCamera;
} ApplicationContext;

#endif //VULKAN_CONTEXT_H