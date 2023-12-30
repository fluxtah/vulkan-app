#include "include/app_camera.h"

void initCamera(Camera *camera, vec3 position, float fov, float aspectRatio, float nearPlane, float farPlane) {
    glm_vec3_copy(position, camera->position); // Set the camera position
    camera->yaw = -90.0f; // Yaw set to -90 degrees to face down the positive Z-axis
    camera->pitch = 0.0f; // Pitch set to 0 degrees, level with the horizon
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up); // Y is up in Vulkan
    camera->fov = fov;
    camera->aspectRatio = aspectRatio;
    camera->nearPlane = nearPlane;
    camera->farPlane = farPlane;
    camera->lookAtTarget = false; // Look-at-target mode disabled by default

    updateCamera(camera);
}

void updateCamera(Camera *camera) {
    if (camera->lookAtTarget) {
        // Look at the target
        glm_lookat(camera->position, camera->target, camera->up, camera->view);
    } else {
        // FPS camera behavior based on yaw and pitch
        vec3 front;
        front[0] = cosf(glm_rad(camera->pitch)) * cosf(glm_rad(camera->yaw));
        front[1] = sinf(glm_rad(camera->pitch));
        front[2] = cosf(glm_rad(camera->pitch)) * sinf(glm_rad(camera->yaw));
        glm_vec3_normalize_to(front, camera->direction); // Normalize and store in camera->direction

        vec3 target;
        glm_vec3_add(camera->position, front, target); // Calculate the look-at target
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
    updateCamera(camera);
}

// Function to disable the look-at-target mode
void disableLookAtTarget(Camera *camera) {
    camera->lookAtTarget = false;
    updateCamera(camera);
}