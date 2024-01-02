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

void printGpuMemoryInfo(VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    printf("Memory Heaps:\n");
    for (uint32_t i = 0; i < memProperties.memoryHeapCount; ++i) {
        printf("Heap %d: \n", i);
        printf("    Size: %llu MB\n", memProperties.memoryHeaps[i].size / (1024 * 1024));

        if (memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            printf("    This heap is local to the device (usually means it's on the GPU).\n");
        } else {
            printf("    This heap is not local to the device (may be system memory).\n");
        }
    }

    printf("\nMemory Types:\n");
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        printf("Memory Type %d: \n", i);
        printf("    Heap Index: %d\n", memProperties.memoryTypes[i].heapIndex);

        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            printf("    Device local (fastest for the GPU)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            printf("    Host visible (can be mapped for host access)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            printf("    Host coherent (host and device memory are coherent)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
            printf("    Host cached (host access to this memory is cached)\n");
        }
    }
}

void printDeviceLimits(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    printf("Device Limits:\n");
    printf("Max Descriptor Sets: %u\n", deviceProperties.limits.maxDescriptorSetUniformBuffers);
    printf("Max Uniform Buffers per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetUniformBuffers);
    printf("Max Dynamic Uniform Buffers per Descriptor Set: %u\n",
           deviceProperties.limits.maxDescriptorSetUniformBuffersDynamic);
    printf("Max Storage Buffers per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetStorageBuffers);
    printf("Max Dynamic Storage Buffers per Descriptor Set: %u\n",
           deviceProperties.limits.maxDescriptorSetStorageBuffersDynamic);
    printf("Max Sampled Images per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetSampledImages);
    printf("Max Storage Images per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetStorageImages);
}