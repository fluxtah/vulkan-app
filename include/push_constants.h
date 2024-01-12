#ifndef GAME_PUSH_CONSTANTS_H
#define GAME_PUSH_CONSTANTS_H
#include <cglm/cglm.h>

typedef struct PushConstants {
        mat4 model;
        mat4 view;
        mat4 proj;
} PushConstants;
#endif //GAME_PUSH_CONSTANTS_H
