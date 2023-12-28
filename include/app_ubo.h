#ifndef APP_UBO_H
#define APP_UBO_H

#include <cglm/cglm.h>

typedef struct {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightDir;   // Direction of the light
    vec4 lightColor; // Color of the light
} UniformBufferObject;
#endif //APP_UBO_H
