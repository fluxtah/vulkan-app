#include "include/vulkan/render.h"

void renderSubmit(VulkanDeviceContext *context, VkSemaphore *waitSemaphores, VkSemaphore *signalSemaphores,
                  VkFence inFlightFence, VkCommandBuffer *commandBuffers, uint32_t commandBufferCount) {
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = commandBufferCount;  // Updated to take commandBufferCount
    submitInfo.pCommandBuffers = commandBuffers;         // Directly use the array of command buffers

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(context->graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        fprintf(stderr, "Failed to submit draw command buffer\n");
        exit(1);
    }
}

void renderPresent(VulkanDeviceContext *context, VkSwapchainKHR swapChain, VkSemaphore *signalSemaphores, uint32_t imageIndex) {
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(context->presentQueue, &presentInfo);
}