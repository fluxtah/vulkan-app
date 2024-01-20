#define CGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION

#include "include/vulkan/setup.h"
#include "include/vulkan/render.h"
#include "include/ubo_update.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_config.h"
#include "include/emitter.h"
#include "include/pipelines/pfx/pfx_pipeline.h"
#include "include/pipelines/pfx/pfx_pipeline_config.h"

static float lastFrameTime = 0.0f;

/**
 * This is the maximum number of entities that can collide with another entity per frame.
 * If this number is exceeded, the remaining entities will not be checked for collisions.
 */
const int MAX_COLLIDING_ENTITIES = 100;

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

    Entity **collidingEntities = malloc(sizeof(void *) * MAX_COLLIDING_ENTITIES);

    //
    // Init Kotlin Application
    //
    ktInitApplication();

    //
    // TEMP PARTICLE EMITTER
    //
    CreateEmitterInfo emitterInfo = {
            .modelFileName = "models/quad-explosion.glb",
            .positionX = 0.0f,
            .positionY = 2.0f,
            .positionZ = 10.0f,
            .scaleX = 1.0f,
            .scaleY = 1.0f,
            .scaleZ = 1.0f,
    };

    Emitter *emitter = createEmitter(context, &emitterInfo);

    /*
     * MAIN LOOP
     */
    while (!glfwWindowShouldClose(context->vulkanDeviceContext->window)) {
        float time = (float) glfwGetTime();
        float deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        glfwPollEvents();

        ktUpdateApplication(time, deltaTime);

        // TODO testing emitter
        emitter->position[0] = 2.0f * sinf(time);
        emitter->position[2] = 8.0f;

        // TODO this will need to be done for each emitter
        applyEmitterChanges(emitter);

        //
        // Get the list of entities we want to render
        //
        EntityArray *ktEntities = (EntityArray *) ktGetEntities();

        //
        // Apply changes/transformations that are needed for rendering
        //
        for (size_t i = 0; i < ktEntities->size; i++) {
            Entity *entity = (Entity *) (ktEntities->entities[i]);
            applyEntityChanges(entity);
        }

        //
        // Collision detection
        //
        for (size_t i = 0; i < ktEntities->size; i++) {

            Entity *sourceEntity = (Entity *) (ktEntities->entities[i]);
            // make an array to hold the entities that collided with the source entity
            // we have to dynamically allocate this array because we don't know how many entities will collide
            // with the source entity
            int collingEntityCount = 0;
            for (size_t j = 0; j < ktEntities->size; j++) {
                Entity *otherEntity = (Entity *) (ktEntities->entities[j]);

                if (otherEntity == sourceEntity) continue;
                if (MAX_COLLIDING_ENTITIES == collingEntityCount) {
                    fprintf(stderr,
                            "Max colliding entities reached, skipping collision detection for remaining entities\n");
                    break;
                }

                if (aabbCollision(&sourceEntity->aabb, &otherEntity->aabb))
                    collidingEntities[collingEntityCount++] = otherEntity->kotlinEntityInfo;
            }
            if (collingEntityCount > 0) {
                ktCollisionCallback(sourceEntity->kotlinEntityInfo, collidingEntities, collingEntityCount);
            }
        }

        recordComputeCommandBuffer(emitter, deltaTime);

        for (size_t i = 0; i < context->vulkanSwapchainContext->swapChainImageCount; i++) {
            beginCommandBufferRecording(
                    context->commandBuffers[i],
                    context->renderPass,
                    context->swapChainFramebuffers[i],
                    &context->vulkanSwapchainContext->swapChainExtent);
            recordCommandBuffer(
                    context->vulkanSwapchainContext->depthStencil,
                    context->commandBuffers[i],
                    context->pipelineConfig->pipeline,
                    context->pipelineConfig->pipelineLayout,
                    ktEntities);

            recordEmitterBuffer(
                    context->commandBuffers[i],
                    emitter->graphicsPipelineConfig,
                    emitter->computePipelineConfig->particleBuffer,
                    emitter
            );

#ifdef DEBUG
            if (context->debugBoundingVolumes) {
                recordDebugCommandBuffer(
                        context->commandBuffers[i],
                        context->debugPipelineConfig->pipeline,
                        context->debugPipelineConfig->pipelineLayout,
                        ktEntities,
                        context->debugCubeBuffer->buffer,
                        context->activeCamera);
            }
#endif

            endCommandBufferRecording(context->commandBuffers[i]);
        }

        //
        // Update UBOs
        //
        for (size_t i = 0; i < ktEntities->size; i++) {
            Entity *obj = (Entity *) ktEntities->entities[i];
            updateTransformUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
            updateLightsUBO(context->vulkanDeviceContext->device, obj, context->activeCamera);
        }

        updateEmitterTransformUBO(context->vulkanDeviceContext->device, emitter, context->activeCamera);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context->vulkanDeviceContext->device, context->vulkanSwapchainContext->swapChain,
                              UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        VkCommandBuffer commandBuffersToSubmit[2]; // Maximum of 1 command buffers
        uint32_t commandBufferCount = 0;

        // Always add the primary command buffer
        commandBuffersToSubmit[commandBufferCount++] = context->commandBuffers[imageIndex];
        commandBuffersToSubmit[commandBufferCount++] = emitter->computePipelineConfig->commandBuffers[0];

        vkResetFences(context->vulkanDeviceContext->device, 1, &inFlightFence);

        renderSubmit(context->vulkanDeviceContext, waitSemaphores, signalSemaphores, inFlightFence,
                     commandBuffersToSubmit, commandBufferCount);


        renderPresent(context->vulkanDeviceContext, context->vulkanSwapchainContext->swapChain, signalSemaphores,
                      imageIndex);

        vkWaitForFences(context->vulkanDeviceContext->device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetCommandPool(context->vulkanDeviceContext->device, context->commandPool, 0);

        free(ktEntities->entities);
        free(ktEntities);
    }

    /*
     * CLEAN UP
     */
    vkWaitForFences(context->vulkanDeviceContext->device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(context->vulkanDeviceContext->device, 1, &inFlightFence);
    vkDestroySemaphore(context->vulkanDeviceContext->device, renderFinishedSemaphore, NULL);
    vkDestroySemaphore(context->vulkanDeviceContext->device, imageAvailableSemaphore, NULL);
    vkDestroyFence(context->vulkanDeviceContext->device, inFlightFence, NULL);

    // TODO TEMP PARTICLE EMITTER
    destroyEmitter(context, emitter);

    ktDestroyApplication();

    destroyApplication(context);

    glfwTerminate();

    return 0;
}
