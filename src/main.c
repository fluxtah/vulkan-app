#define CGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION

#include "include/vulkan/setup.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/framebuffer.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/depth.h"
#include "include/context.h"
#include "include/vulkan/render.h"
#include "include/pipelineconfig.h"

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

    ApplicationContext context;
    ktSetVulkanContext(&context);

    if (setupApplication(&context) == -1) {
        printf("Something went wrong with setting up the application");
        return -1;
    }

    // Create the Kotlin Application
    ktCreateApplication();

    ImageMemory *depthStencil = createDepthStencil(&context);

    VkRenderPass renderPass = createRenderPass(&context);

    context.pipelineConfig = createBasicShaderPipelineConfig(&context, renderPass);
    if (context.pipelineConfig->pipeline == VK_NULL_HANDLE)
        return -1;

    context.swapChainFramebuffers = createSwapChainFramebuffers(&context, context.swapChainImageViews,
                                                                context.swapChainImageCount, renderPass,
                                                                depthStencil->imageView);

    VkCommandBuffer *commandBuffers = allocateCommandBuffers(context.device, context.commandPool,
                                                             context.swapChainImageCount);
    if (commandBuffers == VK_NULL_HANDLE)
        return -1;

    //
    // Init KT
    //
    ktInitApplication();

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    // Semaphore creation for image availability
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(context.device, &semaphoreInfo, NULL, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(context.device, &semaphoreInfo, NULL, &renderFinishedSemaphore) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create semaphores\n");
        return -1;
    }

    // Fence creation for rendering completion
    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(context.device, &fenceInfo, NULL, &inFlightFence) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create fence\n");
        return -1;
    }

    /*
     * MAIN LOOP
     */

    while (!glfwWindowShouldClose(context.window)) {
        float time = (float) glfwGetTime();
        float deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        glfwPollEvents();

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};

        ktUpdateApplication(time, deltaTime);

        //
        // Get the list of entities we want to render
        //w
        EntityArray *ktEntities = (EntityArray *) ktGetEntities();

        int numRenderObjects = ktEntities->size;
        RenderObject *renderObjects[ktEntities->size];

        for (int i = 0; i < ktEntities->size; i++) {
            RenderObject *renderObject = (RenderObject *) (ktEntities->entities[i]);
            renderObjects[i] = renderObject;
        }

        free(ktEntities->entities);
        free(ktEntities);

        for (size_t i = 0; i < context.swapChainImageCount; i++) {
            recordCommandBuffer(
                    commandBuffers[i],
                    renderPass,
                    context.swapChainFramebuffers[i],
                    context.swapChainExtent,
                    context.pipelineConfig->pipeline,
                    context.pipelineConfig->pipelineLayout,
                    renderObjects,
                    numRenderObjects);
        }

        //
        // Update UBOs
        //
        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];
            updateTransformUBO(context.device, obj, context.activeCamera);
            updateLightsUBO(context.device, obj, context.activeCamera);
        }

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context.device, context.swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        vkResetFences(context.device, 1, &inFlightFence);

        renderSubmit(&context, waitSemaphores, signalSemaphores, inFlightFence, commandBuffers, imageIndex);
        renderPresent(&context, context.swapChain, signalSemaphores, imageIndex);

        vkWaitForFences(context.device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    }

    /*
     * CLEAN UP
     */
    vkWaitForFences(context.device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(context.device, 1, &inFlightFence);
    vkDestroySemaphore(context.device, renderFinishedSemaphore, NULL);
    vkDestroySemaphore(context.device, imageAvailableSemaphore, NULL);
    vkDestroyFence(context.device, inFlightFence, NULL);

    for (size_t i = 0; i < context.swapChainImageCount; i++) {
        vkDestroyFramebuffer(context.device, context.swapChainFramebuffers[i], NULL);
    }
    free(context.swapChainFramebuffers);

    vkDestroyRenderPass(context.device, renderPass, NULL);

    destroyImageMemory(&context, depthStencil);

    destroyPipelineConfig(&context, context.pipelineConfig);

    ktDestroyApplication();

    context.activeCamera = NULL;

    destroyApplication(&context);

    glfwTerminate();

    return 0;
}
