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

int setupApplication(ApplicationContext *context);
void destroyApplication(ApplicationContext *context);

VkSurfaceKHR createVulkanSurface(VkInstance instance, GLFWwindow *window);

VkSurfaceFormatKHR getBestSurfaceFormat(ApplicationContext *context);

VkPresentModeKHR getBestPresentMode(ApplicationContext *context);

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t *graphicsQueueFamilyIndex,
                             uint32_t *presentQueueFamilyIndex);

#endif // VULKAN_SETUP_H
