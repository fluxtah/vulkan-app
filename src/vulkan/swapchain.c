#include "include/vulkan/swapchain.h"

VkSwapchainKHR createSwapChain(VulkanContext *context, VkExtent2D *swapchainExtent) {
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physicalDevice, context->surface, &capabilities);

    // Set the swap extent
    *swapchainExtent = capabilities.currentExtent;
    if (swapchainExtent->width == UINT32_MAX) {
        int width, height;
        glfwGetFramebufferSize(context->window, &width, &height);
        swapchainExtent->width = (uint32_t) width;
        swapchainExtent->height = (uint32_t) height;
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = context->surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = context->surfaceFormat.format;
    createInfo.imageColorSpace = context->surfaceFormat.colorSpace;
    createInfo.imageExtent = *swapchainExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = context->presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Other settings like image sharing mode, pre-transform, composite alpha, present mode, clipped, old swap chain
    // ...

    VkSwapchainKHR swapChain;
    VkResult result = vkCreateSwapchainKHR(context->device, &createInfo, NULL, &swapChain);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create swap chain%d\n", result);
        return VK_NULL_HANDLE;
    }

    return swapChain;
}

void createSwapChainImageViews(VkDevice device, VkSwapchainKHR swapChain, VkImageView **swapChainImageViews,
                               uint32_t *imageCount) {
    vkGetSwapchainImagesKHR(device, swapChain, imageCount, NULL);

    VkImage *swapChainImages = malloc(*imageCount * sizeof(VkImage));
    if (!swapChainImages) {
        fprintf(stderr, "Failed to allocate memory for swap chain images\n");
        return;
    }

    if (vkGetSwapchainImagesKHR(device, swapChain, imageCount, swapChainImages) != VK_SUCCESS) {
        fprintf(stderr, "Failed to get swap chain images\n");
        free(swapChainImages);
        return;
    }

    *swapChainImageViews = malloc(*imageCount * sizeof(VkImageView));
    if (!*swapChainImageViews) {
        fprintf(stderr, "Failed to allocate memory for image views\n");
        free(swapChainImages);
        return;
    }

    for (uint32_t i = 0; i < *imageCount; i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Specify the view type
        createInfo.format = VK_FORMAT_B8G8R8A8_UNORM; // Specify the format (adjust as needed)
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, NULL, &((*swapChainImageViews)[i])) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create image view for image %d\n", i);
            // Handle partial cleanup if needed
        }
    }

    free(swapChainImages);
}
