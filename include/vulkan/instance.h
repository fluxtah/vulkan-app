#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include "include/util_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>

VkInstance createVulkanInstance();

#endif //VULKAN_INSTANCE_H
