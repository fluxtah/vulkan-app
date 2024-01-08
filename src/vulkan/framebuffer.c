#include "include/vulkan/framebuffer.h"

VkFramebuffer *createSwapChainFramebuffers(VkDevice device, VulkanSwapchainContext *vulkanSwapchainContext) {
    // Allocate memory for framebuffers
    VkFramebuffer *swapChainFramebuffers = malloc(sizeof(VkFramebuffer) * vulkanSwapchainContext->swapChainImageCount);
    if (swapChainFramebuffers == NULL) {
        fprintf(stderr, "Failed to allocate memory for frame buffers\n");
        return NULL;
    }

    // Create a framebuffer for each swap chain image view
    for (size_t i = 0; i < vulkanSwapchainContext->swapChainImageCount; i++) {
        VkImageView attachments[2] = {
                vulkanSwapchainContext->swapChainImageViews[i], // Color attachment
                vulkanSwapchainContext->depthStencil->imageView          // Depth attachment
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vulkanSwapchainContext->renderPass;
        framebufferInfo.attachmentCount = 2; // Now using 2 attachments: color and depth
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vulkanSwapchainContext->swapChainExtent.width;
        framebufferInfo.height = vulkanSwapchainContext->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create framebuffer\n");
            // Clean up any framebuffers that were successfully created
            for (size_t j = 0; j < i; j++) {
                vkDestroyFramebuffer(device, swapChainFramebuffers[j], NULL);
            }
            free(swapChainFramebuffers);
            return NULL;
        }
    }

    return swapChainFramebuffers;
}
