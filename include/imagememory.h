#ifndef APP_IMAGEMEMORY_H
#define APP_IMAGEMEMORY_H

#include <vulkan/vulkan.h>
#include "include/vulkan/buffer.h"
#include <cglm/cglm.h>
#include "include/vulkan/image.h"
#include "include/vulkan/descriptor.h"
#include "include/ubo.h"
#include "include/modeldata.h"

typedef struct ImageMemory {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView imageView;
} ImageMemory;

void destroyImageMemory(VkDevice device, ImageMemory *imageMemory);

#endif //APP_IMAGEMEMORY_H
