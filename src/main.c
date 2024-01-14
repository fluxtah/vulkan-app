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
#include "include/kotlin.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

static float lastFrameTime = 0.0f;

bool aabbCollision(AABB *a, AABB *b) {
    return (a->min[0] <= b->max[0] && a->max[0] >= b->min[0]) &&
           (a->min[1] <= b->max[1] && a->max[1] >= b->min[1]) &&
           (a->min[2] <= b->max[2] && a->max[2] >= b->min[2]);
}

int main() {
    //
    // Bind kotlin callbacks to C API functions
    //
    bindKotlinApi();

    ktCreateApplication();

    ApplicationContext *context = createApplication();
    ktSetVulkanContext(context);

    if (context == NULL) {
        printf("Something went wrong with setting up the application");
        return -1;
    }

    //
    // Init Kotlin Application
    //
    ktInitApplication();

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    // Semaphore creation for image availability
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(context->vulkanDeviceContext->device, &semaphoreInfo, NULL, &imageAvailableSemaphore) !=
        VK_SUCCESS ||
        vkCreateSemaphore(context->vulkanDeviceContext->device, &semaphoreInfo, NULL, &renderFinishedSemaphore) !=
        VK_SUCCESS) {
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

    Entity **collisionTargetInfos = malloc(sizeof(void *) * 500); // TODO adjust as needed

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

        //
        // Collision detection
        //
        for (size_t i = 0; i < ktEntities->size; i++) {

            Entity *sourceEntity = (Entity *) (ktEntities->entities[i]);
            // make an array to hold the entities that collided with the source entity
            // we have to dynamically allocate this array because we don't know how many entities will collide
            // with the source entity
            int collisionTargetInfosSize = 0;
            for (size_t j = 0; j < ktEntities->size; j++) {
                Entity *otherEntity = (Entity *) (ktEntities->entities[j]);

                if (otherEntity == sourceEntity) continue;

                if (aabbCollision(&sourceEntity->aabb, &otherEntity->aabb))
                    collisionTargetInfos[collisionTargetInfosSize++] = otherEntity->kotlinEntityInfo;
            }
            if (collisionTargetInfosSize > 0) {
                ktCollisionCallback(sourceEntity->kotlinEntityInfo, collisionTargetInfos, collisionTargetInfosSize);
            }
        }

        for (size_t i = 0; i < context->vulkanSwapchainContext->swapChainImageCount; i++) {
            recordCommandBuffer(
                    context->pipelineConfig->commandBuffers[i],
                    context->pipelineConfig->renderPass,
                    context->pipelineConfig->swapChainFramebuffers[i],
                    context->vulkanSwapchainContext->swapChainExtent,
                    context->pipelineConfig->pipeline,
                    context->pipelineConfig->pipelineLayout,
                    ktEntities);

#ifdef DEBUG
            if (context->debugBoundingVolumes) {
                recordDebugCommandBuffer(
                        context->debugPipelineConfig->commandBuffers[i],
                        context->debugPipelineConfig->renderPass,
                        context->debugPipelineConfig->swapChainFramebuffers[i],
                        context->vulkanSwapchainContext->swapChainExtent,
                        context->debugPipelineConfig->pipeline,
                        context->debugPipelineConfig->pipelineLayout,
                        ktEntities,
                        context->debugCubeBuffer->buffer,
                        context->activeCamera);
            }
#endif
        }

        //
        // Update UBOs
        //
        for (size_t i = 0; i < ktEntities->size; i++) {
            Entity *obj = (Entity *) ktEntities->entities[i];
            updateTransformUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
            updateLightsUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
        }

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context->vulkanDeviceContext->device, context->vulkanSwapchainContext->swapChain,
                              UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        VkCommandBuffer commandBuffersToSubmit[2]; // Maximum of 2 command buffers
        uint32_t commandBufferCount = 0;

        // Always add the primary command buffer
        commandBuffersToSubmit[commandBufferCount++] = context->pipelineConfig->commandBuffers[imageIndex];

#ifdef DEBUG
        // Add the debug pipeline's command buffer in debug mode
        if (context->debugBoundingVolumes) {
            commandBuffersToSubmit[commandBufferCount++] = context->debugPipelineConfig->commandBuffers[imageIndex];
        }
#endif

        renderSubmit(context->vulkanDeviceContext, waitSemaphores, signalSemaphores, inFlightFence,
                     commandBuffersToSubmit, commandBufferCount);


        renderPresent(context->vulkanDeviceContext, context->vulkanSwapchainContext->swapChain, signalSemaphores,
                      imageIndex);

        vkWaitForFences(context->vulkanDeviceContext->device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(context->vulkanDeviceContext->device, 1, &inFlightFence);
        vkResetCommandPool(context->vulkanDeviceContext->device, context->commandPool, 0);

        free(ktEntities->entities);
        free(ktEntities);
    }

    /*
     * CLEAN UP
     */
    vkQueueWaitIdle(context->vulkanDeviceContext->graphicsQueue);
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
