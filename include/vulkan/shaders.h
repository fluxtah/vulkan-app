#ifndef VULKAN_SHADERS_H
#define VULKAN_SHADERS_H

#include "include/debug.h"
#include <vulkan/vulkan.h>
#include <stdlib.h>
#include <stdio.h>

VkShaderModule createShaderModule(VkDevice device, const char* filePath);

#endif // VULKAN_SHADERS_H