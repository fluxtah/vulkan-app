#include "include/vulkan/depth.h"

VkFormat findDepthFormat(VkPhysicalDevice physicalDevice) {
    VkFormat formats[] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
    int formatsCount = sizeof(formats) / sizeof(formats[0]);

    for (int i = 0; i < formatsCount; ++i) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, formats[i], &props);

        if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) ==
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            return formats[i];
        }
    }

    fprintf(stderr, "failed to find supported format!");
    exit(1);
}

void
createDepthResources(VulkanContext *context, VkCommandPool commandPool, VkImage *depthImage,
                     VkDeviceMemory *depthImageMemory, VkImageView *depthImageView) {
    VkFormat depthFormat = findDepthFormat(context->physicalDevice);

    // Assuming createImage and createImageView are implemented
    createImage(context->device, context->physicalDevice, context->swapChainExtent.width, context->swapChainExtent.height, depthFormat,
                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);

    *depthImageView = createImageView(context->device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    transitionDepthStencilImageLayout(context->device, commandPool, context->graphicsQueue, *depthImage, depthFormat,
                          VK_IMAGE_LAYOUT_UNDEFINED,
                          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}
