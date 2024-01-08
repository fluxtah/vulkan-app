#include "include/imagememory.h"

void destroyImageMemory(VkDevice device, ImageMemory *imageMemory) {
    vkDestroyImage(device, imageMemory->image, NULL);
    vkDestroyImageView(device, imageMemory->imageView, NULL);
    vkFreeMemory(device, imageMemory->memory, NULL);
    free(imageMemory);
}