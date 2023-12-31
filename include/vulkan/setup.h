#ifndef VULKAN_SETUP_H
#define VULKAN_SETUP_H

#include "include/context.h"
#include "include/window.h"
#include "instance.h"
#include "physicaldevice.h"
#include "commandpool.h"
#include "depth.h"
#include "sampler.h"
#include "include/kotlin.h"
#include "include/vulkan/swapchain.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

ApplicationContext *createApplication();
void destroyApplication(ApplicationContext *context);

VulkanDeviceContext *createVulkanDeviceContext();
void destroyVulkanDeviceContext(VulkanDeviceContext *context);

VulkanSwapchainContext *createVulkanSwapchainContext(VulkanDeviceContext *vulkanDeviceContext, VkCommandPool commandPool);
void destroyVulkanSwapchainContext(VulkanDeviceContext *context, VulkanSwapchainContext *vulkanSwapchainContext);

VkSurfaceKHR createVulkanSurface(VkInstance instance, GLFWwindow *window);

VkSurfaceFormatKHR getBestSurfaceFormat(VulkanDeviceContext *context);

VkPresentModeKHR getBestPresentMode(VulkanDeviceContext *context);

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t *graphicsQueueFamilyIndex,
                             uint32_t *presentQueueFamilyIndex);

#endif // VULKAN_SETUP_H
