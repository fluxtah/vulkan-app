#include "include/light.h"

Light *createLight(CreateLightInfo *info) {
    Light *light = malloc(sizeof(Light));
    light->type = info->type;
    light->intensity = info->intensity;
    glm_vec4_copy((vec4) {info->colorR, info->colorG, info->colorB, info->colorA}, light->color);
    glm_vec3_copy((vec3) {info->positionX, info->positionY, info->positionZ}, light->position);
    glm_vec3_copy((vec3) {info->directionX, info->directionY, info->directionZ}, light->direction);

    return light;
}

void destroyLight(Light *light) {
    free(light);
}
