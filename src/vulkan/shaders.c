#include "include/vulkan/shaders.h"

VkShaderModule createShaderModule(VkDevice device, const char *filePath) {
    // Open the file
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filePath);
        return VK_NULL_HANDLE;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory for the file contents
    char *buffer = (char *) malloc(fileSize);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for file buffer\n");
        fclose(file);
        return VK_NULL_HANDLE;
    }

    // Read the file into the buffer
    fread(buffer, 1, fileSize, file);
    fclose(file);

    // Create the shader module
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fileSize;
    createInfo.pCode = (const uint32_t *) buffer;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
        LOG_ERROR("Failed to create shader module");
        free(buffer);
        return VK_NULL_HANDLE;
    }

    // Free the file buffer
    free(buffer);

    return shaderModule;
}
