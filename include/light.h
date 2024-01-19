#ifndef APP_LIGHT_H
#define APP_LIGHT_H

#include "cglm/cglm.h"
#include "model.h"

typedef struct {
    int type;
    vec4 color;
    vec3 position;
    char _pad1[4]; // shader alignment
    vec3 direction;
    float intensity;
} Light;

Light *createLight(CreateLightInfo *info);
void destroyLight(Light *light);

#endif //APP_LIGHT_H
