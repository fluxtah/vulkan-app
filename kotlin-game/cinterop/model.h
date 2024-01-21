#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <stdbool.h>

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

typedef struct CreateCameraInfo {
    float positionX;
    float positionY;
    float positionZ;
    float    fov;
    float    aspect;
    float    near;
    float    far;
} CreateCameraInfo;

typedef struct CreateLightInfo {
    int type;
    float colorR;
    float colorG;
    float colorB;
    float colorA;
    float positionX;
    float positionY;
    float positionZ;
    float directionX;
    float directionY;
    float directionZ;
    float intensity;
} CreateLightInfo;

typedef struct CreateEntityInfo {
    char *modelFileName;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    bool useOrientedBoundingBox;
} CreateEntityInfo;

typedef struct LightArray {
    void** lights; // Pointer to the first light
    int size;     // Size of the array
} LightArray;

typedef struct EntityArray {
    void** entities; // Pointer to the first entity
    int size;     // Size of the array
} EntityArray;

typedef struct CreateSoundInfo {
    int loop;
} CreateSoundInfo;


typedef struct CreateEmitterInfo {
    char *modelFileName;
    int maxParticles;
    float positionX;
    float positionY;
    float positionZ;
    float rotationX;
    float rotationY;
    float rotationZ;
    float scaleX;
    float scaleY;
    float scaleZ;

    char *computeShaderFileName;
    char *vertexShaderFileName;
    char *fragmentShaderFileName;

} CreateEmitterInfo;

typedef struct EmitterArray {
    void** emitters; // Pointer to the first entity
    int size;     // Size of the array
} EmitterArray;

#endif // MODEL_H