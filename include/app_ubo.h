#ifndef APP_UBO_H
#define APP_UBO_H

#include <cglm/cglm.h>
#include <stdalign.h>

#define MAX_LIGHTS 5

typedef struct {
    mat4 model;
    mat4 view;
    mat4 proj;
} TransformUBO;

typedef struct {
    int type;
    vec4 color;
    vec3 position;
    char _pad1;
    vec3 direction;
    float intensity;
} Light;

typedef struct {
    Light lights[MAX_LIGHTS];
    vec3 cameraPos;
    int numLightsInUse; // should not exceed MAX_LIGHTS!
} LightArrayUBO;


#endif
