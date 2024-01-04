#ifndef APP_CAMERA_H
#define APP_CAMERA_H

#include <cglm/cglm.h>
#include "model.h"

typedef struct Camera {
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

Camera *createCamera(CreateCameraInfo *info);
void destroyCamera(Camera *camera);
void applyCameraChanges(Camera *camera);
void setLookAtTarget(Camera *camera, vec3 target);
void disableLookAtTarget(Camera *camera);
void moveCameraForward(Camera *camera, float speed);
void moveCameraBackward(Camera *camera, float speed);
void moveCameraLeft(Camera *camera, float speed);
void moveCameraRight(Camera *camera, float speed);
void pitchCamera(Camera *camera, float amount);
void yawCamera(Camera *camera, float amount);
void positionCamera(Camera *camera, float x, float y, float z);
void setCameraLookAt(Camera *camera, float x, float y, float z);

#endif //APP_CAMERA_H
