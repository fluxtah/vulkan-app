#ifndef APP_PIPELINES_PFX_PARTICLE_H
#define APP_PIPELINES_PFX_PARTICLE_H

#include "cglm/cglm.h"

typedef struct Particle {
    vec3 position;
    vec3 velocity;
    float lifeTime;
    // Add more per-particle attributes if needed
} Particle;

#endif //APP_PIPELINES_PFX_PARTICLE_H
