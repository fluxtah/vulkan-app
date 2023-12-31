#include "include/ubo_update.h"

void updateLightsUBO(VkDevice device, Entity *entity, Camera *camera) {
    LightArray *ktLights = (LightArray *) ktGetLights();

    if (!ktLights) return;

    Light lights[ktLights->size];

    for (int i = 0; i < ktLights->size; i++) {
        Light *light = (Light *) (ktLights->lights[i]);
        lights[i] = *light;
    }

    LightingUBO lightingUBO = {0};
    if(ktLights->size > MAX_LIGHTS) {
        lightingUBO.numLightsInUse = MAX_LIGHTS;
    } else {
        lightingUBO.numLightsInUse = ktLights->size;
    }

    memcpy(lightingUBO.lights, lights, sizeof(Light) * lightingUBO.numLightsInUse);
    glm_vec3_copy(camera->position, lightingUBO.cameraPos);
    glm_vec3_copy((vec3) {0.04f, 0.04f, 0.04f}, lightingUBO.ambientLightColor);

    void *lightsData;
    vkMapMemory(device, entity->lightingUBO->memory, 0, sizeof(LightingUBO), 0, &lightsData);
    memcpy(lightsData, &lightingUBO, sizeof(LightingUBO));
    vkUnmapMemory(device, entity->lightingUBO->memory);

    free(ktLights->lights);
    free(ktLights);
}

void updateTransformUBO(VkDevice device, Entity *entity, Camera *camera) {
    TransformUBO transformUBO = {0};
    glm_mat4_identity(transformUBO.model);
    glm_scale(transformUBO.model, entity->scale);
    glm_translate(transformUBO.model, entity->position);
    glm_rotate(transformUBO.model, glm_rad(entity->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(transformUBO.model, glm_rad(entity->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(transformUBO.model, glm_rad(entity->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation

    memcpy(transformUBO.view, camera->view, sizeof(mat4));
    memcpy(transformUBO.proj, camera->proj, sizeof(mat4));

    void *transformData;
    vkMapMemory(device, entity->transformUBO->memory, 0, sizeof(TransformUBO), 0, &transformData);
    memcpy(transformData, &transformUBO, sizeof(TransformUBO));
    vkUnmapMemory(device, entity->transformUBO->memory);
}
