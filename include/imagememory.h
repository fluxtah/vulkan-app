#ifndef APP_IMAGEMEMORY_H
#define APP_IMAGEMEMORY_H

#include <vulkan/vulkan.h>
#include <cglm/cglm.h>
#include "include/vulkan/image.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/buffer.h"
#include "include/ubo.h"
#include "include/modeldata.h"

typedef struct {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView imageView;
} ImageMemory;

void destroyImageMemory(ApplicationContext *context, ImageMemory *imageMemory);
#endif //APP_IMAGEMEMORY_H
