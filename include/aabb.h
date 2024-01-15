//
// Created by Ian Warwick on 12/01/2024.
//

#ifndef GAME_AABB_H
#define GAME_AABB_H

#include <cglm/vec3.h>

typedef struct AABB {
    vec3 min; // Minimum corner of the AABB
    vec3 max; // Maximum corner of the AABB
} AABB;

bool aabbCollision(AABB *a, AABB *b);

#endif //GAME_AABB_H
