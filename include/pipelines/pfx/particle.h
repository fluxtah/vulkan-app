#ifndef APP_PIPELINES_PFX_PARTICLE_H
#define APP_PIPELINES_PFX_PARTICLE_H

#include "cglm/cglm.h"

typedef struct Particle {
    vec3 position;
    char _pad[4];
    vec3 velocity;
    char _pad2[4];
    vec3 scale;
    float lifeTime;
    // Add more per-particle attributes if needed
} Particle;

#endif //APP_PIPELINES_PFX_PARTICLE_H
