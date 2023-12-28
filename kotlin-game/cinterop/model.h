#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>

typedef struct Extent2D {
    uint32_t    width;
    uint32_t    height;
} Extent2D;

typedef struct Extent3D {
    uint32_t    width;
    uint32_t    height;
    uint32_t    depth;
} Extent3D;

typedef struct Offset2D {
    int32_t    x;
    int32_t    y;
} Offset2D;

typedef struct Offset3D {
    int32_t    x;
    int32_t    y;
    int32_t    z;
} Offset3D;

typedef struct Rect2D {
    Offset2D    offset;
    Extent2D    extent;
} Rect2D;

typedef struct Viewport {
    float    x;
    float    y;
    float    width;
    float    height;
    float    minDepth;
    float    maxDepth;
} Viewport;

#endif // MODEL_H