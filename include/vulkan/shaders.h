#ifndef VULKAN_SHADERS_H
#define VULKAN_SHADERS_H

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

VkShaderModule createShaderModule(VkDevice device, const char* filePath);
void destroyShaderModule(VkDevice device, VkShaderModule shaderModule);

#endif // VULKAN_SHADERS_H