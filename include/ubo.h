#ifndef APP_UBO_H
#define APP_UBO_H

#include <cglm/cglm.h>
#include <stdalign.h>
#include "light.h"

#define MAX_LIGHTS 5

typedef struct {
    mat4 model;
    mat4 view;
    mat4 proj;
} TransformUBO;

typedef struct {
    Light lights[MAX_LIGHTS];
    vec3 ambientLightColor;
    char _pad1; // shader alignment
    vec3 cameraPos;
    int numLightsInUse; // should not exceed MAX_LIGHTS!
} LightingUBO;

#endif
