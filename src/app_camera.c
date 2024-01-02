#include "include/app_camera.h"


Camera *createCamera(CreateCameraInfo *info) {
    Camera *camera = malloc(sizeof(Camera));
    vec3 position = {info->positionX, info->positionY, info->positionZ};
    glm_vec3_copy(position, camera->position); // Set the camera position
    camera->yaw = -90.0f; // Yaw set to -90 degrees to face down the positive Z-axis
    camera->pitch = 0.0f; // Pitch set to 0 degrees, level with the horizon
    glm_vec3_copy((vec3) {0.0f, 1.0f, 0.0f}, camera->up); // Y is up in Vulkan
    camera->fov = info->fov;
    camera->aspectRatio = info->aspect;
    camera->nearPlane = info->near;
    camera->farPlane = info->far;
    camera->lookAtTarget = false; // Look-at-target mode disabled by default

    updateCamera(camera);

    return camera;
}

void updateCamera(Camera *camera) {

    // FPS camera behavior based on yaw and pitch
    // Constrain the pitch
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    vec3 front;
    front[0] = cosf(glm_rad(camera->pitch)) * cosf(glm_rad(camera->yaw));
    front[1] = sinf(glm_rad(camera->pitch));
    front[2] = cosf(glm_rad(camera->pitch)) * sinf(glm_rad(camera->yaw));
    glm_vec3_normalize_to(front, camera->direction); // Normalize and store in camera->direction

    vec3 target;
    glm_vec3_add(camera->position, front, target); // Calculate the look-at target

    if (camera->lookAtTarget) {
        glm_lookat(camera->position, camera->target, camera->up, camera->view); // Create the view matrix
    } else {
        glm_lookat(camera->position, target, camera->up, camera->view); // Create the view matrix
    }
    // Update the projection matrix
    glm_perspective(glm_rad(camera->fov), camera->aspectRatio, camera->nearPlane, camera->farPlane, camera->proj);
    camera->proj[1][1] *= -1; // Adjust for Vulkan's clip space

//    printf("Camera Direction: %f, %f, %f\n", camera->direction[0], camera->direction[1], camera->direction[2]);
}

void setLookAtTarget(Camera *camera, vec3 target) {
    glm_vec3_copy(target, camera->target);
    camera->lookAtTarget = true;
}

// Function to disable the look-at-target mode
void disableLookAtTarget(Camera *camera) {
    camera->lookAtTarget = false;
}

void moveCameraForward(Camera *camera, float amount) {
    // Move forward
    vec3 delta;
    glm_vec3_scale(camera->direction, amount, delta);
    glm_vec3_add(camera->position, delta, camera->position);
}

void moveCameraBackward(Camera *camera, float amount) {
    vec3 delta;
    glm_vec3_scale(camera->direction, amount, delta);
    glm_vec3_sub(camera->position, delta, camera->position);
}

void moveCameraLeft(Camera *camera, float amount) {
    vec3 left, right;
    glm_vec3_crossn(camera->direction, camera->up, right);
    glm_vec3_negate_to(right, left); // Negate the right vector to get left
    glm_vec3_scale(left, amount, left);
    glm_vec3_add(camera->position, left, camera->position);
}

void moveCameraRight(Camera *camera, float amount) {
    vec3 right;
    glm_vec3_crossn(camera->direction, camera->up, right);
    glm_vec3_scale(right, amount, right);
    glm_vec3_add(camera->position, right, camera->position);
}

void pitchCameraUp(Camera *camera, float amount) {
    camera->pitch += amount;
}

void pitchCameraDown(Camera *camera, float amount) {
    camera->pitch -= amount;
}

void yawCameraLeft(Camera *camera, float amount) {
    camera->yaw -= amount;
}

void yawCameraRight(Camera *camera, float amount) {
    camera->yaw += amount;
}

void destroyCamera(Camera *camera) {
    free(camera);
}
