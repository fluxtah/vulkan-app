#ifndef APP_KOTLIN_H
#define APP_KOTLIN_H

#include "include/sound.h"
#include "include/light.h"
#include "include/camera.h"
#include "include/renderresources.h"
#include "libkotlin_game_api.h"

bool keys[1024];
void bindKotlinApi();
int isKeyPressed(int key);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif //APP_KOTLIN_H
