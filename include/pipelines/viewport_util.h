#ifndef APP_PIPELINES_VIEWPORT_UTIL_H
#define APP_PIPELINES_VIEWPORT_UTIL_H

#include <vulkan/vulkan.h>
#include "kotlin-game/cinterop/model.h"

VkViewport viewportToVkViewport(Viewport *viewport);
VkRect2D viewportToScissor(Viewport *viewport);
#endif //APP_PIPELINES_VIEWPORT_UTIL_H
