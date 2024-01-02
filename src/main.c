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
#include "include/app_ubo.h"
#include "include/app_renderobject.h"
#include "include/app_camera.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

// Global or static variable
static float lastFrameTime = 0.0f;
static bool keys[1024];

void printGpuMemoryInfo(VkPhysicalDevice physicalDevice);

void printDeviceLimits(VkPhysicalDevice device);

void initLight(Light *light, float *color, float *position, float *direction, float intensity);

void updateLightsUBO(VkDevice device, RenderObject *renderObject, Camera *camera, Light *lights, int numLights) {
    // Set up lighting information
    LightingUBO lightingUBO = {0};
    lightingUBO.numLightsInUse = numLights;
    memcpy(lightingUBO.lights, lights, sizeof(Light) * lightingUBO.numLightsInUse);
    glm_vec3_copy(camera->position, lightingUBO.cameraPos);
    glm_vec3_copy((vec3) {0.04f, 0.04f, 0.04f}, lightingUBO.ambientLightColor);

    void *lightsData;
    vkMapMemory(device, renderObject->lightingUBO->memory, 0, sizeof(LightingUBO), 0, &lightsData);
    memcpy(lightsData, &lightingUBO, sizeof(LightingUBO));
    vkUnmapMemory(device, renderObject->lightingUBO->memory);
}

void updateTransformUBO(VkDevice device, RenderObject *renderObject, Camera *camera) {
    TransformUBO transformUBO = {0};
    glm_mat4_identity(transformUBO.model);
    glm_scale(transformUBO.model, renderObject->scale);
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation
    glm_translate(transformUBO.model, renderObject->position);

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

void updateCameraMovement(VulkanContext *context, float deltaTime) {
    const float baseSpeed = 3.0f; // Adjust bases speed as needed
    float cameraSpeed = baseSpeed * deltaTime;
    const float rotationSpeed = 40.0f; // Rotation speed
    float cameraRotationSpeed = rotationSpeed * deltaTime;

    if (keys[GLFW_KEY_W]) {
        moveCameraForward(context->activeCamera, cameraSpeed);
    }
    if (keys[GLFW_KEY_S]) {
        moveCameraBackward(context->activeCamera, cameraSpeed);
    }
    if (keys[GLFW_KEY_A]) {
        moveCameraLeft(context->activeCamera, cameraSpeed);
    }
    if (keys[GLFW_KEY_D]) {
        moveCameraRight(context->activeCamera, cameraSpeed);
    }

    // Rotation controls
    if (keys[GLFW_KEY_UP]) {
        pitchCameraUp(context->activeCamera, cameraRotationSpeed);
    }
    if (keys[GLFW_KEY_DOWN]) {
        pitchCameraDown(context->activeCamera, cameraRotationSpeed);
    }
    if (keys[GLFW_KEY_LEFT]) {
        yawCameraLeft(context->activeCamera, cameraRotationSpeed);
    }
    if (keys[GLFW_KEY_RIGHT]) {
        yawCameraRight(context->activeCamera, cameraRotationSpeed);
    }

    applyCameraChanges(context->activeCamera); // Update the camera's view matrix

    //  printf("deltaTime: %f, cameraSpeed: %f, cameraRotationSpeed: %f\n", deltaTime, cameraSpeed, cameraRotationSpeed);

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
    // Camera
    ktSetCreateCameraFunc(createCamera);
    ktSetDestroyCameraFunc(destroyCamera);
    ktSetMoveCameraForwardFunc(moveCameraForward);
    ktSetMoveCameraBackwardFunc(moveCameraBackward);
    ktSetMoveCameraLeftFunc(moveCameraLeft);
    ktSetMoveCameraRightFunc(moveCameraRight);
    ktSetPitchCameraUpFunc(pitchCameraUp);
    ktSetPitchCameraDownFunc(pitchCameraDown);
    ktSetApplyCameraChangesFunc(applyCameraChanges);
    ktSetActiveCameraFunc(setActiveCamera);
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
    VkDescriptorSetLayout vertexShaderDescriptorSetLayout = createVertexShaderDescriptorSetLayout(context.device);
    VkDescriptorSetLayout fragmentShaderDescriptorSetLayout = createFragmentShaderDescriptorSetLayout(context.device);

    //
    // Create a descriptor pool
    //
    VkDescriptorPool descriptorPool;
    createBasicShaderDescriptorPool(context.device, &descriptorPool);

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    createDepthResources(&context, context.commandPool, &depthImage, &depthImageMemory,
                         &depthImageView);

    VkRenderPass renderPass = createRenderPass(&context);
    VkShaderModule vertexShaderModule = createShaderModule(context.device, "shaders/basic.vert.spv");
    VkShaderModule fragmentShaderModule = createShaderModule(context.device, "shaders/basic.frag.spv");

    VkPipelineLayout pipelineLayout = createPipelineLayout(context.device,
                                                           vertexShaderDescriptorSetLayout,
                                                           fragmentShaderDescriptorSetLayout);
    Viewport viewport = (Viewport) {
            0, 0, context.swapChainExtent.width, context.swapChainExtent.height, 0.0f, 1.0f
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
    // Camera
    //
//    CreateCameraInfo createCameraInfo = {
//            0.0f,
//            0.0f,
//            2.0f,
//            45.0f,
//            (float) context.swapChainExtent.width / (float) context.swapChainExtent.height,
//            0.1f,
//            100.0f
//    };
//    setActiveCamera(&context, createCamera(&createCameraInfo));

    glfwSetKeyCallback(context.window, key_callback);

    //
    // Lights
    //
    Light light = {0};
    initLight(
            &light,
            (vec4) {1.0f, 1.0f, 1.0f, 1.0f},
            (vec3) {0.0f, 1.0f, 1.7f},
            (vec3) {0.0f, 0.0f, -1.0f},
            1.0f
    );

    Light light2 = {0};
    initLight(
            &light2,
            (vec4) {0.0f, 1.0f, 0.0f, 1.0f},
            (vec3) {-2.0f, 1.0f, 1.7f},
            (vec3) {0.0f, 0.0f, 1.0f},
            1.0f
    );

    int numLights = 1;
    Light lights[] = {light, light2};

    //
    // Scene Objects
    //
    int numRenderObjects = 2;
    RenderObject **renderObjects = malloc(sizeof(RenderObject *) * numRenderObjects);
    if (!renderObjects) {
        printf("unable to allocate memory for render objects pointers\n");
        exit(1);
    }

    renderObjects[0] = createRenderObjectFromFile(
            &context,
            vertexShaderDescriptorSetLayout,
            fragmentShaderDescriptorSetLayout,
            descriptorPool,
            "models/plane.glb");
    renderObjects[1] = createRenderObjectFromFile(
            &context,
            vertexShaderDescriptorSetLayout,
            fragmentShaderDescriptorSetLayout,
            descriptorPool,
            "models/sphere.glb");

    glm_vec3_copy((vec3) {0.0f, 0.0f, 0.0f}, renderObjects[0]->position);
    glm_vec3_copy((vec3) {0.0f, 0.0f, 0.0f}, renderObjects[1]->position);
    glm_vec3_copy((vec3) {1.0f, 1.0f, 1.0f}, renderObjects[1]->scale);


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

        // Move
        renderObjects[1]->rotation[1] += 20.5f * deltaTime;
        renderObjects[1]->rotation[2] += 20.5f * deltaTime;
        updateCameraMovement(&context, deltaTime);

        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];

            // Update UBO
            updateTransformUBO(context.device, obj, context.activeCamera);
            updateLightsUBO(context.device, obj, context.activeCamera, lights, numLights);
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

    vkDestroyDescriptorPool(context.device, descriptorPool, NULL);
    vkDestroyDescriptorSetLayout(context.device, vertexShaderDescriptorSetLayout, NULL);
    vkDestroyDescriptorSetLayout(context.device, fragmentShaderDescriptorSetLayout, NULL);

    for (size_t i = 0; i < numRenderObjects; i++) {
        destroyRenderObject(&context, renderObjects[i]);
    }
    free(renderObjects);

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

    destroyCamera(context.activeCamera);

    vkDestroySwapchainKHR(context.device, context.swapChain, NULL);

    destroyVulkan(&context);

    glfwTerminate();

    return 0;
}

void initLight(Light *light, float *color, float *position, float *direction, float intensity) {
    light->type = 1;
    light->intensity = intensity;
    glm_vec4_copy(color, (*light).color);
    glm_vec3_copy(position, (*light).position);
    if (direction != NULL) {
        glm_vec3_copy(direction, (*light).direction);
    }
}

void printGpuMemoryInfo(VkPhysicalDevice physicalDevice) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    printf("Memory Heaps:\n");
    for (uint32_t i = 0; i < memProperties.memoryHeapCount; ++i) {
        printf("Heap %d: \n", i);
        printf("    Size: %llu MB\n", memProperties.memoryHeaps[i].size / (1024 * 1024));

        if (memProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            printf("    This heap is local to the device (usually means it's on the GPU).\n");
        } else {
            printf("    This heap is not local to the device (may be system memory).\n");
        }
    }

    printf("\nMemory Types:\n");
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
        printf("Memory Type %d: \n", i);
        printf("    Heap Index: %d\n", memProperties.memoryTypes[i].heapIndex);

        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            printf("    Device local (fastest for the GPU)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            printf("    Host visible (can be mapped for host access)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            printf("    Host coherent (host and device memory are coherent)\n");
        }
        if (memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
            printf("    Host cached (host access to this memory is cached)\n");
        }
    }
}

void printDeviceLimits(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    printf("Device Limits:\n");
    printf("Max Descriptor Sets: %u\n", deviceProperties.limits.maxDescriptorSetUniformBuffers);
    printf("Max Uniform Buffers per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetUniformBuffers);
    printf("Max Dynamic Uniform Buffers per Descriptor Set: %u\n",
           deviceProperties.limits.maxDescriptorSetUniformBuffersDynamic);
    printf("Max Storage Buffers per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetStorageBuffers);
    printf("Max Dynamic Storage Buffers per Descriptor Set: %u\n",
           deviceProperties.limits.maxDescriptorSetStorageBuffersDynamic);
    printf("Max Sampled Images per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetSampledImages);
    printf("Max Storage Images per Descriptor Set: %u\n", deviceProperties.limits.maxDescriptorSetStorageImages);
}