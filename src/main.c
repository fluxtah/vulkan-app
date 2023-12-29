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

void updateUniformBuffer(VkDevice device, RenderObject *renderObject, Camera *camera) {
    UniformBufferObject ubo = {0};
    glm_mat4_identity(ubo.model);

    // Apply transformations based on RenderObject's properties
    glm_translate(ubo.model, renderObject->position);
    glm_scale(ubo.model, renderObject->scale);
    glm_rotate(ubo.model, glm_rad(renderObject->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(ubo.model, glm_rad(renderObject->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(ubo.model, glm_rad(renderObject->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    memcpy(ubo.view, camera->view, sizeof(mat4));
    memcpy(ubo.proj, camera->proj, sizeof(mat4));

    // Set up lighting information
    // Example values - replace with your actual lighting data
    vec3 lightDirection = {0.0f, 1.0f, 0.0f}; // Direction of light
    vec4 lightColor = {1.0f, 1.0f, 1.0f, 1.0f}; // Color of light

    glm_vec3_copy(lightDirection, ubo.lightDir);
    glm_vec4_copy(lightColor, ubo.lightColor);

    // Map memory, copy data, and unmap
    void *data;
    vkMapMemory(device, renderObject->uniformBuffer->memory, 0, sizeof(UniformBufferObject), 0, &data);
    memcpy(data, &ubo, sizeof(UniformBufferObject));
    vkUnmapMemory(device, renderObject->uniformBuffer->memory);
}

int main() {
    // Register Kotlin Application callbacks
    ktSetCreateShaderModuleFunc(createShaderModule);
    ktSetDestroyShaderModuleFunc(destroyShaderModule);

    ktSetCreatePipelineLayoutFunc(createPipelineLayout);
    ktSetDestroyPipelineLayoutFunc(destroyPipelineLayout);

    ktSetCreatePipelineFunc(createPipeline);
    ktSetDestroyPipelineFunc(destroyPipeline);

    ktSetCreateRenderPassFunc(createRenderPass);
    ktSetDestroyRenderPassFunc(destroyRenderPass);

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
    ktSetVkDevice(context.device);

    //
    // VULKAN RENDERING
    //
    swapChain = createSwapChain(&context, &swapChainExtent);
    if (swapChain == VK_NULL_HANDLE)
        return -1;

    ktSetVkSwapChain(swapChain);
    ktSetSwapChainExtent((Extent2D *) &swapChainExtent);

    createSwapChainImageViews(context.device, swapChain, &swapChainImageViews, &imageCount);

    //
    // Create a descriptor set layout (common for all objects)
    //
    VkDescriptorSetLayout descriptorSetLayout;
    createBasicShaderDescriptorSetLayout(context.device, &descriptorSetLayout);
    ktSetVkDescriptorSetLayout(descriptorSetLayout);

    //
    // Create a descriptor pool (common for all objects)
    //
    VkDescriptorPool descriptorPool;
    createBasicShaderDescriptorPool(context.device, &descriptorPool);

    Camera camera;
    initCamera(&camera,
               (vec3) {0.0f, 3.0f, 3.0f}, // Camera position
               (vec3) {0.0f, 0.0f, 0.0f}, // Camera looking at the origin
               (vec3) {0.0f, 1.0f, 0.0f}, // Up vector
               60.0f,                     // FOV
               (float) swapChainExtent.width / (float) swapChainExtent.height,           // Aspect ratio
               0.1f,                      // Near plane
               100.0f);                   // Far plane

    int numRenderObjects = 2;
    RenderObject **renderObjects = malloc(sizeof(RenderObject *) * numRenderObjects);
    if (!renderObjects) {
        printf("unable to allocate memory for render objects pointers\n");
        exit(1);
    }

    renderObjects[0] = createRenderObjectFromFile(&context, descriptorSetLayout, descriptorPool, "models/plane.glb");
    renderObjects[1] = createRenderObjectFromFile(&context, descriptorSetLayout, descriptorPool, "models/ship.glb");

    glm_vec3_copy((vec3) {0.0f, 0.0f, 0.0f}, renderObjects[0]->position);
    glm_vec3_copy((vec3) {0.0f, 0.2f, -2.2f}, renderObjects[1]->position);
    glm_vec3_copy((vec3) {0.5f, 0.5f, 0.5f}, renderObjects[1]->scale);

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    createDepthResources(&context, swapChainExtent, context.commandPool, &depthImage, &depthImageMemory,
                         &depthImageView);

    ktInitApplication();

    swapChainFramebuffers = createSwapChainFramebuffers(context.device, swapChainImageViews, imageCount,
                                                        ktGetVkRenderPass("default"), swapChainExtent, depthImageView);

    VkPipeline graphicsPipeline = ktGetVkPipeline("default");
    if (graphicsPipeline == VK_NULL_HANDLE)
        return -1;

    VkCommandBuffer *commandBuffers = allocateCommandBuffers(context.device, context.commandPool, imageCount);
    if (commandBuffers == VK_NULL_HANDLE)
        return -1;


    for (size_t i = 0; i < imageCount; i++) {
        recordCommandBuffer(commandBuffers[i], ktGetVkRenderPass("default"), swapChainFramebuffers[i], swapChainExtent,
                            graphicsPipeline, ktGetVkPipelineLayout("default"), renderObjects, numRenderObjects);
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
        renderObjects[1]->rotation[1] += 0.3f;
   //     renderObjects[1]->rotation[2] += 0.3f;

        for (size_t i = 0; i < numRenderObjects; i++) {
            RenderObject *obj = renderObjects[i];

            // Update UBO
            updateUniformBuffer(context.device, obj, &camera);
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
    vkDestroyDescriptorSetLayout(context.device, descriptorSetLayout, NULL);

    for (size_t i = 0; i < numRenderObjects; i++) {
        destroyRenderObject(&context, renderObjects[i]);
    }
    free(renderObjects);

    for (size_t i = 0; i < imageCount; i++) {
        vkDestroyFramebuffer(context.device, swapChainFramebuffers[i], NULL);
    }
    free(swapChainFramebuffers);

    ktDestroyAllPipelines();
    ktDestroyAllShaderModules();
    ktDestroyAllRenderPasses();

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
