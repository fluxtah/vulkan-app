#include "include/vulkan/physicaldevice.h"

void printDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

    if (extensionCount == 0)
    {
        printf("No device extensions found.\n");
        return;
    }

    VkExtensionProperties *extensions = (VkExtensionProperties *)malloc(extensionCount * sizeof(VkExtensionProperties));
    if (extensions == NULL)
    {
        printf("Failed to allocate memory for extension properties.\n");
        return;
    }

    VkResult result = vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, extensions);
    if (result != VK_SUCCESS)
    {
        printf("Failed to enumerate device extensions.\n");
        free(extensions);
        return;
    }

    printf("Device Extensions (%d):\n", extensionCount);
    for (uint32_t i = 0; i < extensionCount; i++)
    {
        printf("   %d: %s (version %d)\n", i, extensions[i].extensionName, extensions[i].specVersion);
    }

    free(extensions);
}

int isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    // Check for VK_KHR_swapchain support
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
    VkExtensionProperties *extensions = (VkExtensionProperties *)malloc(extensionCount * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, extensions);

    int swapChainSupported = 0;
    for (uint32_t i = 0; i < extensionCount; i++)
    {
        if (strcmp(extensions[i].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
        {
            swapChainSupported = 1;
            break;
        }
    }

    free(extensions);

    // Check for queue families support
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    int isSuitable = 0;
    if (swapChainSupported)
    {
        for (uint32_t i = 0; i < queueFamilyCount; i++)
        {
            VkBool32 presentSupport = 0;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && presentSupport)
            {
                isSuitable = 1;
                break;
            }
        }
    }

    free(queueFamilies);
    return isSuitable;
}

VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if (deviceCount == 0)
    {
        fprintf(stderr, "Failed to find GPUs with Vulkan support\n");
        return VK_NULL_HANDLE;
    }

    VkPhysicalDevice *devices = (VkPhysicalDevice *)malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    VkPhysicalDevice suitableDevice = VK_NULL_HANDLE;
    for (uint32_t i = 0; i < deviceCount; i++)
    {
        if (isDeviceSuitable(devices[i], surface))
        {
            suitableDevice = devices[i];
            break;
        }
    }

    free(devices);
    if (suitableDevice == VK_NULL_HANDLE)
    {
        fprintf(stderr, "Failed to find a suitable GPU\n");
    }

    printDeviceExtensions(suitableDevice);

    return suitableDevice;
}