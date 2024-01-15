#include "include/pipelines/viewport_util.h"

VkViewport viewportToVkViewport(Viewport *viewport) {
    return (VkViewport) {
            .x = (*viewport).x,
            .y = (*viewport).y,
            .width = (*viewport).width,
            .height = (*viewport).height,
            .minDepth = (*viewport).minDepth,
            .maxDepth = (*viewport).maxDepth
    };
}

VkRect2D viewportToScissor(Viewport *viewport) {
    return (VkRect2D) {
            .offset = {0, 0},         // Initialize the offset
            .extent = (VkExtent2D) {
                    .width = (uint32_t) (*viewport).width,
                    .height = (uint32_t) (*viewport).height
            }
    };
}
