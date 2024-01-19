#ifndef APP_UBO_H
#define APP_UBO_H

#include <cglm/cglm.h>
#include <stdalign.h>
#include "light.h"

#define MAX_LIGHTS 5

typedef struct TransformUBO {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
} TransformUBO;

typedef struct LightingUBO {
    Light lights[MAX_LIGHTS];
    vec3 ambientLightColor;
    char _pad1[4]; // Alignment for vec3
    vec3 cameraPos;
    int numLightsInUse; // should not exceed MAX_LIGHTS!
} LightingUBO;

#endif
