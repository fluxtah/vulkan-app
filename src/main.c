#define CGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "include/vulkan/physicaldevice.h"
#include "include/vulkan/setup.h"
#include "include/vulkan/swapchain.h"
#include "include/vulkan/renderpass.h"
#include "include/vulkan/shaders.h"
#include "include/vulkan/pipeline.h"
#include "include/vulkan/framebuffer.h"
#include "include/vulkan/commandbuffer.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/depth.h"
#include "include/vulkan/context.h"
#include "include/vulkan/render.h"

#include "kotlin-game/build/bin/native/releaseShared/libkotlin_game_api.h"
#include "kotlin-game/cinterop/model.h"
#include "include/ubo.h"
#include "include/renderobject.h"
#include "include/camera.h"
#include "include/light.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

// Global or static variable
static float lastFrameTime = 0.0f;
static bool keys[1024];

void updateLightsUBO(VkDevice device, RenderObject *renderObject, Camera *camera) {
    LightArray *ktLights = (LightArray *) ktGetLights();

    if (!ktLights) return;

    Light lights[ktLights->size];

    for (int i = 0; i < ktLights->size; i++) {
        Light *light = (Light *) (ktLights->lights[i]);
        lights[i] = *light;
    }

    LightingUBO lightingUBO = {0};
    lightingUBO.numLightsInUse = ktLights->size;
    memcpy(lightingUBO.lights, lights, sizeof(Light) * lightingUBO.numLightsInUse);
    glm_vec3_copy(camera->position, lightingUBO.cameraPos);
    glm_vec3_copy((vec3) {0.04f, 0.04f, 0.04f}, lightingUBO.ambientLightColor);

    void *lightsData;
    vkMapMemory(device, renderObject->lightingUBO->memory, 0, sizeof(LightingUBO), 0, &lightsData);
    memcpy(lightsData, &lightingUBO, sizeof(LightingUBO));
    vkUnmapMemory(device, renderObject->lightingUBO->memory);

    free(ktLights->lights);
    free(ktLights);
}

void updateTransformUBO(VkDevice device, RenderObject *renderObject, Camera *camera) {
    TransformUBO transformUBO = {0};
    glm_mat4_identity(transformUBO.model);
    glm_scale(transformUBO.model, renderObject->scale);
    glm_translate(transformUBO.model, renderObject->position);
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    memcpy(transformUBO.view, camera->view, sizeof(mat4));
    memcpy(transformUBO.proj, camera->proj, sizeof(mat4));

    void *transformData;
    vkMapMemory(device, renderObject->transformUBO->memory, 0, sizeof(TransformUBO), 0, &transformData);
    memcpy(transformData, &transformUBO, sizeof(TransformUBO));
    vkUnmapMemory(device, renderObject->transformUBO->memory);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

int isKeyPressed(int key) {
    if (keys[key]) {
        return 1;
    }

    return -1;
}

void setActiveCamera(VulkanContext *context, Camera *camera) {
    context->activeCamera = camera;

    // Default to extent
    if (camera->aspectRatio == 0.0f) {
        camera->aspectRatio = (float) context->swapChainExtent.width / (float) context->swapChainExtent.height;
        applyCameraChanges(camera);
    }
}

void bindKotlinApi() {
    // Input
    ktSetIsKeyPressedFunc(isKeyPressed);

    // Camera
    ktSetCreateCameraFunc(createCamera);
    ktSetDestroyCameraFunc(destroyCamera);
    ktSetMoveCameraForwardFunc(moveCameraForward);
    ktSetMoveCameraBackwardFunc(moveCameraBackward);
    ktSetMoveCameraLeftFunc(moveCameraLeft);
    ktSetMoveCameraRightFunc(moveCameraRight);
    ktSetPitchCameraUpFunc(pitchCameraUp);
    ktSetPitchCameraDownFunc(pitchCameraDown);
    ktSetYawCameraLeftFunc(yawCameraLeft);
    ktSetYawCameraRightFunc(yawCameraRight);
    ktSetApplyCameraChangesFunc(applyCameraChanges);
    ktSetActiveCameraFunc(setActiveCamera);

    // Light
    ktSetCreateLightFunc(createLight);
    ktSetDestroyLightFunc(destroyLight);

    // Entity
    ktSetCreateEntityFunc(createRenderObjectFromFile);
    ktSetDestroyEntityFunc(destroyRenderObject);
    ktSetRotateEntityFunc(rotateRenderObject);
}

int main() {
    bindKotlinApi();

    // Create the Kotlin Application
    ktCreateApplication();

    VulkanContext context;
    if (setupVulkan(&context) == -1) {
        printf("Something went wrong with setting up Vulkan");
        return -1;
    }

    glfwSetKeyCallback(context.window, key_callback);

    printGpuMemoryInfo(context.physicalDevice);
    printDeviceLimits(context.physicalDevice);

    ktSetVulkanContext(&context);

    context.swapChain = createSwapChain(&context);
    if (context.swapChain == VK_NULL_HANDLE)
        return -1;

    createSwapChainImageViews(&context);

    //
    // Create descriptor set layouts
    //
    context.vertexShaderDescriptorSetLayout = createVertexShaderDescriptorSetLayout(context.device);
    context.fragmentShaderDescriptorSetLayout = createFragmentShaderDescriptorSetLayout(context.device);

    //
    // Create a descriptor pool
    //
    context.descriptorPool = createBasicShaderDescriptorPool(context.device);

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    createDepthResources(&context, context.commandPool, &depthImage, &depthImageMemory, &depthImageView);

    VkRenderPass renderPass = createRenderPass(&context);
    VkShaderModule vertexShaderModule = createShaderModule(context.device, "shaders/basic.vert.spv");
    VkShaderModule fragmentShaderModule = createShaderModule(context.device, "shaders/basic.frag.spv");

    VkPipelineLayout pipelineLayout = createPipelineLayout(context.device,
                                                           context.vertexShaderDescriptorSetLayout,
                                                           context.fragmentShaderDescriptorSetLayout);
    Viewport viewport = (Viewport) {
            0, 0,
            (float) context.swapChainExtent.width,
            (float) context.swapChainExtent.height,
            0.0f,
            1.0f
    };
    VkPipeline pipeline = createPipeline(context.device, pipelineLayout, renderPass, viewport, vertexShaderModule,
                                         fragmentShaderModule);
    context.swapChainFramebuffers = createSwapChainFramebuffers(&context, context.swapChainImageViews,
                                                                context.swapChainImageCount, renderPass,
                                                                depthImageView);

    if (pipeline == VK_NULL_HANDLE)
        return -1;

    VkCommandBuffer *commandBuffers = allocateCommandBuffers(context.device, context.commandPool,
                                                             context.swapChainImageCount);
    if (commandBuffers == VK_NULL_HANDLE)
        return -1;

    //
    // Init KT
    //
    ktInitApplication();

    //
    // Scene Objects
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

    for (size_t i = 0; i < context.swapChainImageCount; i++) {
        recordCommandBuffer(
                commandBuffers[i],
                renderPass,
                context.swapChainFramebuffers[i],
                context.swapChainExtent,
                pipeline,
                pipelineLayout,
                renderObjects,
                numRenderObjects);
    }

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
        float currentFrameTime = (float) glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glfwPollEvents();

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};

        ktUpdateApplication(currentFrameTime, deltaTime);

        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];

            // Update UBO
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

    vkDestroyDescriptorPool(context.device, context.descriptorPool, NULL);
    vkDestroyDescriptorSetLayout(context.device, context.vertexShaderDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(context.device, context.fragmentShaderDescriptorSetLayout, NULL);

    for (size_t i = 0; i < context.swapChainImageCount; i++) {
        vkDestroyFramebuffer(context.device, context.swapChainFramebuffers[i], NULL);
    }
    free(context.swapChainFramebuffers);

    vkDestroyPipeline(context.device, pipeline, NULL);
    vkDestroyPipelineLayout(context.device, pipelineLayout, NULL);
    vkDestroyShaderModule(context.device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context.device, fragmentShaderModule, NULL);
    vkDestroyRenderPass(context.device, renderPass, NULL);

    vkDestroyImageView(context.device, depthImageView, NULL);
    vkDestroyImage(context.device, depthImage, NULL);
    vkFreeMemory(context.device, depthImageMemory, NULL);

    for (uint32_t i = 0; i < context.swapChainImageCount; i++) {
        vkDestroyImageView(context.device, context.swapChainImageViews[i], NULL);
    }
    free(context.swapChainImageViews);

    ktDestroyApplication();

    context.activeCamera = NULL;

    vkDestroySwapchainKHR(context.device, context.swapChain, NULL);

    destroyVulkan(&context);

    glfwTerminate();

    return 0;
}
