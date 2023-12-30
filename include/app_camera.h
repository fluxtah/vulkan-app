#ifndef APP_CAMERA_H
#define APP_CAMERA_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 up;
    vec3 target; // Target position for look-at-target mode
    float yaw;
    float pitch;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    mat4 view;
    mat4 proj;
    bool lookAtTarget; // Flag to enable look-at-target mode
} Camera;

void initCamera(Camera *camera, vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane);

void updateCamera(Camera *camera);

void setLookAtTarget(Camera *camera, vec3 target);

void disableLookAtTarget(Camera *camera);


#endif //APP_CAMERA_H
