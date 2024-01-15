#include "include/aabb.h"

bool aabbCollision(AABB *a, AABB *b) {
    return (a->min[0] <= b->max[0] && a->max[0] >= b->min[0]) &&
           (a->min[1] <= b->max[1] && a->max[1] >= b->min[1]) &&
           (a->min[2] <= b->max[2] && a->max[2] >= b->min[2]);
}