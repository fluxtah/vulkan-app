#ifndef APP_VULKAN_DEVICE_CONTEXT_H
#define APP_VULKAN_DEVICE_CONTEXT_H

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>

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
#endif //APP_VULKAN_DEVICE_CONTEXT_H
