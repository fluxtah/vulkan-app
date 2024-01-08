#include "include/vulkan/setup.h"
#include "include/vulkan/framebuffer.h"
#include "include/pipelineconfig.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/commandbuffer.h"

VkSurfaceKHR createVulkanSurface(VkInstance instance, GLFWwindow *window) {
    VkSurfaceKHR surface;
    VkResult createSurfaceResult = glfwCreateWindowSurface(instance, window, NULL, &surface);
    if (createSurfaceResult != VK_SUCCESS) {
        fprintf(stderr, "Failed to create surface, code%d\n", createSurfaceResult);
        return VK_NULL_HANDLE;
    }

    return surface;
}

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
                             uint32_t *graphicsQueueFamilyIndex, uint32_t *presentQueueFamilyIndex) {

    if (!graphicsQueueFamilyIndex || !presentQueueFamilyIndex) {
        fprintf(stderr, "Null pointer provided for queue family indices, pass -1 for defaults\n");
        return VK_NULL_HANDLE;
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    *graphicsQueueFamilyIndex = -1;
    *presentQueueFamilyIndex = -1;

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *graphicsQueueFamilyIndex = i;
        }

        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
            *presentQueueFamilyIndex = i;
        }

        if (*graphicsQueueFamilyIndex != -1 && *presentQueueFamilyIndex != -1) {
            break;
        }
    }

    free(queueFamilies);

    if (*graphicsQueueFamilyIndex == -1 || *presentQueueFamilyIndex == -1) {
        fprintf(stderr, "Failed to find suitable queue families\n");
        return VK_NULL_HANDLE;
    }

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    // Define Queue Create Info
    float queuePriority = 1.0f;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = *graphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    static const char *deviceExtensions[] = {VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceFeatures deviceFeatures = {};
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    // Create the Logical Device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.enabledExtensionCount = 2;
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    if (deviceFeatures.samplerAnisotropy) {
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceFeatures.samplerAnisotropy = VK_TRUE;  // Enable the feature
    } else {
        fprintf(stderr, "Device does not support samplerAnisotropy!\n");
        exit(1);
        // Handle the lack of support for anisotropy. You might choose to continue without it or fail to create the device.
    }

    VkDevice device;
    int createLogicalDeviceResult = vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device);
    if (createLogicalDeviceResult != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device %d\n", createLogicalDeviceResult);
        return VK_NULL_HANDLE;
    }

    return device;
}

VkSurfaceFormatKHR getBestSurfaceFormat(VulkanDeviceContext *context) {
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &formatCount, NULL);
    VkSurfaceFormatKHR *formats = malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &formatCount, formats);

    // Choose the best surface format
    VkSurfaceFormatKHR surfaceFormat = formats[0];
    for (uint32_t i = 0; i < formatCount; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
            formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            surfaceFormat = formats[i];
            break;
        }
    }

    free(formats);

    return surfaceFormat;
}

VkPresentModeKHR getBestPresentMode(VulkanDeviceContext *context) {
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &presentModeCount, NULL);
    VkPresentModeKHR *presentModes = malloc(presentModeCount * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &presentModeCount,
                                              presentModes);

    // Choose the best present mode
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32_t i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = presentModes[i];
            break;
        }
    }

    free(presentModes);

    return presentMode;
}

VulkanDeviceContext *createVulkanDeviceContext() {
    VulkanDeviceContext *context = malloc(sizeof(VulkanDeviceContext));

    context->window = initWindow();
    if (!context->window)
        return NULL;

    context->instance = createVulkanInstance();
    if (context->instance == VK_NULL_HANDLE)
        return NULL;

    context->surface = createVulkanSurface(context->instance, context->window);
    if (context->surface == VK_NULL_HANDLE)
        return NULL;

    context->physicalDevice = pickPhysicalDevice(context->instance, context->surface);
    if (context->physicalDevice == VK_NULL_HANDLE)
        return NULL;

    printGpuMemoryInfo(context->physicalDevice);
    printDeviceLimits(context->physicalDevice);

    context->device = createLogicalDevice(context->physicalDevice, context->surface, &context->graphicsQueueFamilyIndex,
                                          &context->presentQueueFamilyIndex);
    if (context->device == VK_NULL_HANDLE)
        return NULL;

    printf("graphicsQueueFamilyIndex %d\n", context->graphicsQueueFamilyIndex);
    printf("presentQueueFamilyIndex %d\n", context->presentQueueFamilyIndex);

    vkGetDeviceQueue(context->device, context->graphicsQueueFamilyIndex, 0, &context->graphicsQueue);
    vkGetDeviceQueue(context->device, context->presentQueueFamilyIndex, 0, &context->presentQueue);

    context->depthFormat = findDepthFormat(context->physicalDevice);
    context->surfaceFormat = getBestSurfaceFormat(context);
    context->presentMode = getBestPresentMode(context);

    return context;
}

void destroyVulkanDeviceContext(VulkanDeviceContext *context) {
    vkDestroySurfaceKHR(context->instance, context->surface, NULL);
    vkDestroyDevice(context->device, NULL);
    vkDestroyInstance(context->instance, NULL);
    glfwDestroyWindow(context->window);
    free(context);
}

VulkanSwapchainContext *
createVulkanSwapchainContext(VulkanDeviceContext *vulkanDeviceContext, VkCommandPool commandPool) {
    VulkanSwapchainContext *vulkanSwapchainContext = malloc(sizeof(VulkanSwapchainContext));

    CreateSwapChainResult result = {0};
    if (createSwapChain(vulkanDeviceContext, &result) != 0) {
        printf("Unable to create swapchain");
        return NULL;
    }

    vulkanSwapchainContext->swapChain = result.swapChain;
    vulkanSwapchainContext->swapChainExtent = result.swapChainExtent;


    vulkanSwapchainContext->swapChainImageViews = createSwapChainImageViews(
            vulkanDeviceContext->device,
            vulkanSwapchainContext
    );

    if (vulkanSwapchainContext->swapChainImageViews == VK_NULL_HANDLE) {
        printf("Unable to create swapchain image views");
        return NULL;
    }

    vulkanSwapchainContext->depthStencil = createDepthStencil(vulkanDeviceContext, commandPool,
                                                              vulkanSwapchainContext->swapChainExtent);
    vulkanSwapchainContext->renderPass = createRenderPass(vulkanDeviceContext);
    vulkanSwapchainContext->swapChainFramebuffers = createSwapChainFramebuffers(vulkanDeviceContext->device,
                                                                                vulkanSwapchainContext);

    return vulkanSwapchainContext;
}

void destroyVulkanSwapchainContext(VulkanDeviceContext *context, VulkanSwapchainContext *vulkanSwapchainContext) {
    for (size_t i = 0; i < vulkanSwapchainContext->swapChainImageCount; i++) {
        vkDestroyFramebuffer(context->device, vulkanSwapchainContext->swapChainFramebuffers[i], NULL);
    }
    free(vulkanSwapchainContext->swapChainFramebuffers);

    vkDestroyRenderPass(context->device, vulkanSwapchainContext->renderPass, NULL);

    destroyImageMemory(context->device, vulkanSwapchainContext->depthStencil);

    for (uint32_t i = 0; i < vulkanSwapchainContext->swapChainImageCount; i++) {
        vkDestroyImageView(context->device, vulkanSwapchainContext->swapChainImageViews[i], NULL);
    }
    free(vulkanSwapchainContext->swapChainImageViews);
    vkDestroySwapchainKHR(context->device, vulkanSwapchainContext->swapChain, NULL);
    free(vulkanSwapchainContext);
}

ApplicationContext *createApplication() {
    ApplicationContext *applicationContext = malloc(sizeof(ApplicationContext));

    VulkanDeviceContext *vulkanDeviceContext = createVulkanDeviceContext();
    applicationContext->vulkanDeviceContext = vulkanDeviceContext;

    applicationContext->commandPool = createCommandPool(vulkanDeviceContext->device,
                                                        vulkanDeviceContext->graphicsQueueFamilyIndex);

    VulkanSwapchainContext *vulkanSwapchainContext = createVulkanSwapchainContext(vulkanDeviceContext,
                                                                                  applicationContext->commandPool);
    applicationContext->vulkanSwapchainContext = vulkanSwapchainContext;

    if (applicationContext->commandPool == VK_NULL_HANDLE)
        return NULL;

    createTextureSampler(applicationContext->vulkanDeviceContext->device, &applicationContext->sampler);

    applicationContext->audioContext = createAudioContext();

    glfwSetKeyCallback(vulkanDeviceContext->window, key_callback);

    applicationContext->pipelineConfig = createBasicShaderPipelineConfig(vulkanDeviceContext,
                                                                         vulkanSwapchainContext->renderPass,
                                                                         vulkanSwapchainContext->swapChainExtent);

    applicationContext->commandBuffers = allocateCommandBuffers(vulkanDeviceContext->device,
                                                                applicationContext->commandPool,
                                                                vulkanSwapchainContext->swapChainImageCount);

    if (applicationContext->commandBuffers == VK_NULL_HANDLE)
        return NULL;

    return applicationContext;
}

void destroyApplication(ApplicationContext *context) {
    context->activeCamera = NULL;
    destroyPipelineConfig(context->vulkanDeviceContext, context->pipelineConfig);
    destroyAudioContext(context->audioContext);
    vkDestroySampler(context->vulkanDeviceContext->device, context->sampler, NULL);
    vkDestroyCommandPool(context->vulkanDeviceContext->device, context->commandPool, NULL);
    destroyVulkanSwapchainContext(context->vulkanDeviceContext, context->vulkanSwapchainContext);
    destroyVulkanDeviceContext(context->vulkanDeviceContext);

    free(context);
}
