#ifndef APP_UBO_UPDATE_H
#define APP_UBO_UPDATE_H


#include "ubo.h"
#include "entity.h"
#include "camera.h"
#include "libkotlin_game_api.h"
#include <vulkan/vulkan.h>

void updateLightsUBO(VkDevice device, Entity *entity, Camera *camera);
void updateTransformUBO(VkDevice device, Entity *entity, Camera *camera);
#endif // APP_UBO_UPDATE_H
