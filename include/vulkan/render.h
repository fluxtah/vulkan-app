#ifndef VULKAN_RENDER_H
#define VULKAN_RENDER_H
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <stdlib.h>
#include "include/context.h"

void renderSubmit(ApplicationContext *context, VkSemaphore *waitSemaphores, VkSemaphore *signalSemaphores,
                  VkFence inFlightFence, VkCommandBuffer *commandBuffers, uint32_t imageIndex);
void renderPresent(ApplicationContext *context, VkSwapchainKHR swapChain, VkSemaphore *signalSemaphores, uint32_t imageIndex);

#endif //VULKAN_RENDER_H
