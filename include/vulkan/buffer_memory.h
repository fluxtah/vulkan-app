//
// Created by Ian Warwick on 12/01/2024.
//

#ifndef GAME_BUFFER_MEMORY_H
#define GAME_BUFFER_MEMORY_H

#include <vulkan/vulkan.h>
typedef struct {
    VkBuffer buffer;
    VkDeviceMemory memory;
} BufferMemory;

#endif //GAME_BUFFER_MEMORY_H
