#ifndef GAME_OBB_H
#define GAME_OBB_H

#include <cglm/vec3.h>

typedef struct OBB {
    vec3 center;   // Center of the OBB
    vec3 extents;  // Half-lengths along each axis
    mat4 transform;
} OBB;
#endif //GAME_OBB_H
