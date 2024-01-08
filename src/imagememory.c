#include "include/imagememory.h"

void destroyImageMemory(ApplicationContext *context, ImageMemory *imageMemory) {
    vkDestroyImage(context->device, imageMemory->image, NULL);
    vkDestroyImageView(context->device, imageMemory->imageView, NULL);
    vkFreeMemory(context->device, imageMemory->memory, NULL);
    free(imageMemory);
}