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

// TODO refactor this so we don't have top level variables!
//  - Move everything in main() into a runApplication() class
// Re-creatable resources (screen changes)
VkExtent2D swapChainExtent;
VkSwapchainKHR swapChain;
VkImageView *swapChainImageViews;
uint32_t imageCount;
VkFramebuffer *swapChainFramebuffers;

void printGpuMemoryInfo(VkPhysicalDevice physicalDevice);

void printDeviceLimits(VkPhysicalDevice device);

void updateLightsUBO(VkDevice device, RenderObject *renderObject, Camera *camera, float time) {
    // Set up lighting information
    LightingUBO lightingUBO = {0};
    Light light = {0};
    light.type = 1;
    glm_vec4_copy((vec4) {1.0f, 1.0f, 1.0f, 1.0f}, light.color);
    glm_vec3_copy((vec3) {0.0f, 1.0f, 2.5f}, light.position);
    glm_vec3_copy((vec3) {0.0f, 0.0f, 0.0f}, light.direction);
    light.intensity = 1.0f;

    Light light2 = {0};
    light2.type = 1;
    glm_vec4_copy((vec4) {1.0f, 1.0f, 1.0f, 1.0f}, light2.color);
    glm_vec3_copy((vec3) {-5.0f, 0.0f, 0.0f}, light2.position);
    glm_vec3_copy((vec3) {0.0f, 0.0f, 0.0f}, light2.direction);
    light2.intensity = 1.0f;

    Light lights[] = {light, light2};
    lightingUBO.numLightsInUse = 1;
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
    glm_translate(transformUBO.model, renderObject->position);
    glm_scale(transformUBO.model, renderObject->scale);
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

int main() {
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

    swapChain = createSwapChain(&context, &swapChainExtent);
    if (swapChain == VK_NULL_HANDLE)
        return -1;

    createSwapChainImageViews(context.device, swapChain, &swapChainImageViews, &imageCount);

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
    createDepthResources(&context, swapChainExtent, context.commandPool, &depthImage, &depthImageMemory,
                         &depthImageView);

    VkRenderPass renderPass = createRenderPass(&context);
    VkShaderModule vertexShaderModule = createShaderModule(context.device, "shaders/basic.vert.spv");
    VkShaderModule fragmentShaderModule = createShaderModule(context.device, "shaders/basic.frag.spv");

    VkPipelineLayout pipelineLayout = createPipelineLayout(context.device,
                                                           vertexShaderDescriptorSetLayout,
                                                           fragmentShaderDescriptorSetLayout);
    Viewport viewport = (Viewport) {
            0, 0, swapChainExtent.width, swapChainExtent.height, 0.0f, 1.0f
    };
    VkPipeline pipeline = createPipeline(context.device, pipelineLayout, renderPass, viewport, vertexShaderModule,
                                         fragmentShaderModule);
    swapChainFramebuffers = createSwapChainFramebuffers(context.device, swapChainImageViews, imageCount,
                                                        renderPass, swapChainExtent, depthImageView);

    if (pipeline == VK_NULL_HANDLE)
        return -1;

    VkCommandBuffer *commandBuffers = allocateCommandBuffers(context.device, context.commandPool, imageCount);
    if (commandBuffers == VK_NULL_HANDLE)
        return -1;

    //
    // Camera
    //
    Camera camera;
    initCamera(&camera,
               (vec3) {0.0f, 1.0f, 2.0f}, // Camera position
               (vec3) {0.0f, 0.0f, 0.0f}, // Camera looking at the origin
               (vec3) {0.0f, 1.0f, 0.0f}, // Up vector
               45.0f,                     // FOV
               (float) swapChainExtent.width / (float) swapChainExtent.height,           // Aspect ratio
               0.1f,                      // Near plane
               100.0f);                   // Far plane

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


    for (size_t i = 0; i < imageCount; i++) {
        recordCommandBuffer(
                commandBuffers[i],
                renderPass,
                swapChainFramebuffers[i],
                swapChainExtent,
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
        glfwPollEvents();

        float currentTime = (float) glfwGetTime();
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};

        // Move
        // renderObjects[1]->rotation[0] += 0.7f;
        renderObjects[1]->rotation[1] += 0.1f;
        renderObjects[1]->rotation[2] += 0.02f;

        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];

            // Update UBO
            updateTransformUBO(context.device, obj, &camera);
            updateLightsUBO(context.device, obj, &camera, currentTime);
        }

        uint32_t imageIndex;
        vkAcquireNextImageKHR(context.device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        vkResetFences(context.device, 1, &inFlightFence);

        renderSubmit(&context, waitSemaphores, signalSemaphores, inFlightFence, commandBuffers, imageIndex);
        renderPresent(&context, swapChain, signalSemaphores, imageIndex);

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

    for (size_t i = 0; i < imageCount; i++) {
        vkDestroyFramebuffer(context.device, swapChainFramebuffers[i], NULL);
    }
    free(swapChainFramebuffers);

    vkDestroyPipeline(context.device, pipeline, NULL);
    vkDestroyPipelineLayout(context.device, pipelineLayout, NULL);
    vkDestroyShaderModule(context.device, vertexShaderModule, NULL);
    vkDestroyShaderModule(context.device, fragmentShaderModule, NULL);
    vkDestroyRenderPass(context.device, renderPass, NULL);

    vkDestroyImageView(context.device, depthImageView, NULL);
    vkDestroyImage(context.device, depthImage, NULL);
    vkFreeMemory(context.device, depthImageMemory, NULL);

    for (uint32_t i = 0; i < imageCount; i++) {
        vkDestroyImageView(context.device, swapChainImageViews[i], NULL);
    }
    free(swapChainImageViews);

    vkDestroySwapchainKHR(context.device, swapChain, NULL);

    destroyVulkan(&context);

    glfwTerminate();

    return 0;
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
