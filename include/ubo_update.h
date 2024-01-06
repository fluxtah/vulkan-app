#ifndef APP_UBO_UPDATE_H
#define APP_UBO_UPDATE_H


#include "ubo.h"
#include "renderobject.h"
#include "camera.h"
#include "libkotlin_game_api.h"
#include <vulkan/vulkan.h>

void updateLightsUBO(VkDevice device, RenderObject *renderObject, Camera *camera);
void updateTransformUBO(VkDevice device, RenderObject *renderObject, Camera *camera);
#endif // APP_UBO_UPDATE_H
