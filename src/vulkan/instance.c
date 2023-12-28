#include "include/vulkan/instance.h"

void printInstanceCreateInfo(const VkInstanceCreateInfo *createInfo) {
    if (createInfo == NULL || createInfo->pApplicationInfo == NULL) {
        printf("Instance Create Info or Application Info is NULL\n");
        return;
    }

    printf("Instance Info:\n");
    const VkApplicationInfo *appInfo = createInfo->pApplicationInfo;
    printf("   Application Name: %s\n", appInfo->pApplicationName);
    printf("   Application Version: %d.%d.%d\n",
           VK_VERSION_MAJOR(appInfo->applicationVersion),
           VK_VERSION_MINOR(appInfo->applicationVersion),
           VK_VERSION_PATCH(appInfo->applicationVersion));
    printf("   Engine Name: %s\n", appInfo->pEngineName);
    printf("   Engine Version: %d.%d.%d\n",
           VK_VERSION_MAJOR(appInfo->engineVersion),
           VK_VERSION_MINOR(appInfo->engineVersion),
           VK_VERSION_PATCH(appInfo->engineVersion));
    printf("   API Version: %d.%d.%d\n",
           VK_VERSION_MAJOR(appInfo->apiVersion),
           VK_VERSION_MINOR(appInfo->apiVersion),
           VK_VERSION_PATCH(appInfo->apiVersion));
}

VkInstance createVulkanInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    uint32_t extraExtensionCount = 2;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    const char *extraExtensions[] = {VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
                                     VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME};
    const char **allExtensions = addStringsToArray(glfwExtensions, glfwExtensionCount, extraExtensions,
                                                   extraExtensionCount);
    printf("Instance Extensions: \n");
    for (unsigned int i = 0; i < glfwExtensionCount + extraExtensionCount; i++) {
        printf("   %s\n", allExtensions[i]);
    }

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount + extraExtensionCount;
    createInfo.ppEnabledExtensionNames = allExtensions;
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.enabledLayerCount = 0;
    printInstanceCreateInfo(&createInfo);

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance: %d\n", result);
        glfwTerminate();
        return VK_NULL_HANDLE;
    }

    free(allExtensions);

    return instance;
}