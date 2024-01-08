#define CGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION

#include "include/vulkan/setup.h"
#include "include/vulkan/commandbuffer.h"
#include "include/context.h"
#include "include/vulkan/render.h"

#include "libkotlin_game_api.h"
#include "kotlin-game/cinterop/model.h"
#include "include/ubo_update.h"
#include "include/renderobject.h"
#include "include/kotlin.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static float lastFrameTime = 0.0f;

int main() {
    //
    // Bind kotlin callbacks to C API functions
    //
    bindKotlinApi();

    ApplicationContext *context = createApplication();
    ktSetVulkanContext(context);

    if (context == NULL) {
        printf("Something went wrong with setting up the application");
        return -1;
    }

    //
    // Init Kotlin Application
    //
    ktCreateApplication();
    ktInitApplication();

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    // Semaphore creation for image availability
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(context->vulkanDeviceContext->device, &semaphoreInfo, NULL, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(context->vulkanDeviceContext->device, &semaphoreInfo, NULL, &renderFinishedSemaphore) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create semaphores\n");
        return -1;
    }

    // Fence creation for rendering completion
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(context->vulkanDeviceContext->device, &fenceInfo, NULL, &inFlightFence) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create fence\n");
        return -1;
    }

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};

    /*
     * MAIN LOOP
     */

    while (!glfwWindowShouldClose(context->vulkanDeviceContext->window)) {
        float time = (float) glfwGetTime();
        float deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        glfwPollEvents();

        ktUpdateApplication(time, deltaTime);

        //
        // Get the list of entities we want to render
        //
        EntityArray *ktEntities = (EntityArray *) ktGetEntities();

        int numRenderObjects = ktEntities->size;
        RenderObject *renderObjects[ktEntities->size];

        for (int i = 0; i < ktEntities->size; i++) {
            RenderObject *renderObject = (RenderObject *) (ktEntities->entities[i]);
            renderObjects[i] = renderObject;
        }

        free(ktEntities->entities);
        free(ktEntities);

        for (size_t i = 0; i < context->vulkanSwapchainContext->swapChainImageCount; i++) {
            recordCommandBuffer(
                    context->commandBuffers[i],
                    context->vulkanSwapchainContext->renderPass,
                    context->vulkanSwapchainContext->swapChainFramebuffers[i],
                    context->vulkanSwapchainContext->swapChainExtent,
                    context->pipelineConfig->pipeline,
                    context->pipelineConfig->pipelineLayout,
                    renderObjects,
                    numRenderObjects);
        }

        //
        // Update UBOs
        //
        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];
            updateTransformUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
            updateLightsUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
        }

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context->vulkanDeviceContext->device, context->vulkanSwapchainContext->swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        vkResetFences(context->vulkanDeviceContext->device, 1, &inFlightFence);

        renderSubmit(context->vulkanDeviceContext, waitSemaphores, signalSemaphores, inFlightFence, context->commandBuffers, imageIndex);
        renderPresent(context->vulkanDeviceContext,  context->vulkanSwapchainContext->swapChain, signalSemaphores, imageIndex);

        vkWaitForFences(context->vulkanDeviceContext->device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    }

    /*
     * CLEAN UP
     */
    vkWaitForFences(context->vulkanDeviceContext->device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(context->vulkanDeviceContext->device, 1, &inFlightFence);
    vkDestroySemaphore(context->vulkanDeviceContext->device, renderFinishedSemaphore, NULL);
    vkDestroySemaphore(context->vulkanDeviceContext->device, imageAvailableSemaphore, NULL);
    vkDestroyFence(context->vulkanDeviceContext->device, inFlightFence, NULL);

    ktDestroyApplication();

    destroyApplication(context);

    glfwTerminate();

    return 0;
}
