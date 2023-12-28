#ifndef VULKAN_SETUP_H
#define VULKAN_SETUP_H

#include "context.h"
#include "include/app_window.h"
#include "instance.h"
#include "physicaldevice.h"
#include "commandpool.h"
#include "depth.h"
#include "sampler.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

int setupVulkan(VulkanContext *context);
void destroyVulkan(VulkanContext *context);

VkSurfaceKHR createVulkanSurface(VkInstance instance, GLFWwindow *window);

VkSurfaceFormatKHR getBestSurfaceFormat(VulkanContext *context);

VkPresentModeKHR getBestPresentMode(VulkanContext *context);

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t *graphicsQueueFamilyIndex,
                             uint32_t *presentQueueFamilyIndex);

#endif // VULKAN_SETUP_H
