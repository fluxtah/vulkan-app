#ifndef VULKAN_PHYSICALDEVICE_H
#define VULKAN_PHYSICALDEVICE_H

#include "include/util_strings.h"
#include "vulkan/vulkan.h"
#include <stdio.h>
#include <stdlib.h>

VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

#endif //VULKAN_PHYSICALDEVICE_H
