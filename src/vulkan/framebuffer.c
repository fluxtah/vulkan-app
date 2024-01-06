#include "include/vulkan/framebuffer.h"

VkFramebuffer *createSwapChainFramebuffers(ApplicationContext *context, VkImageView *swapChainImageViews, uint32_t imageCount,
                                           VkRenderPass renderPass, VkImageView depthImageView) {
    // Allocate memory for framebuffers
    VkFramebuffer *swapChainFramebuffers = malloc(sizeof(VkFramebuffer) * imageCount);
    if (swapChainFramebuffers == NULL) {
        fprintf(stderr, "Failed to allocate memory for frame buffers\n");
        return NULL;
    }

    // Create a framebuffer for each swap chain image view
    for (size_t i = 0; i < imageCount; i++) {
        VkImageView attachments[2] = {
                swapChainImageViews[i], // Color attachment
                depthImageView          // Depth attachment
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 2; // Now using 2 attachments: color and depth
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = context->swapChainExtent.width;
        framebufferInfo.height = context->swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(context->device, &framebufferInfo, NULL, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            fprintf(stderr, "Failed to create framebuffer\n");
            // Clean up any framebuffers that were successfully created
            for (size_t j = 0; j < i; j++) {
                vkDestroyFramebuffer(context->device, swapChainFramebuffers[j], NULL);
            }
            free(swapChainFramebuffers);
            return NULL;
        }
    }

    return swapChainFramebuffers;
}
