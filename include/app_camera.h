#ifndef APP_CAMERA_H
#define APP_CAMERA_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 up;
    float fov;          // Field of view in degrees
    float aspectRatio;
    float nearPlane;
    float farPlane;
    mat4 view;
    mat4 proj;
} Camera;

void initCamera(Camera *camera, vec3 position, vec3 target, vec3 up, float fov, float aspectRatio, float nearPlane,
                float farPlane);

void updateCamera(Camera *camera);

#endif //APP_CAMERA_H
