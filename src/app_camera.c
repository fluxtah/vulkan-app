#include "include/app_camera.h"

void initCamera(Camera *camera, vec3 position, vec3 target, vec3 up, float fov, float aspectRatio, float nearPlane, float farPlane) {
    glm_vec3_copy(position, camera->position);
    glm_vec3_sub(position, target, camera->direction);
    glm_vec3_normalize(camera->direction);
    glm_vec3_copy(up, camera->up);
    camera->fov = fov;
    camera->aspectRatio = aspectRatio;
    camera->nearPlane = nearPlane;
    camera->farPlane = farPlane;

    // Initialize view and projection matrices
    glm_lookat(camera->position, target, camera->up, camera->view);
    glm_perspective(glm_rad(camera->fov), camera->aspectRatio, camera->nearPlane, camera->farPlane, camera->proj);
    camera->proj[1][1] *= -1; // Adjust for Vulkan's clip space
}

void updateCamera(Camera *camera) {
    glm_lookat(camera->position, camera->direction, camera->up, camera->view);
    glm_perspective(glm_rad(camera->fov), camera->aspectRatio, camera->nearPlane, camera->farPlane, camera->proj);
    camera->proj[1][1] *= -1; // Adjust for Vulkan's clip space
}
