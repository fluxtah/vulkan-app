#include "include/ubo.h"

void updateLightsUBO(VkDevice device, RenderObject *renderObject, Camera *camera) {
    LightArray *ktLights = (LightArray* )ktGetLights();

    Light lights[ktLights->size];

    for (int i = 0; i < ktLights->size; i++) {
        Light *light = (Light *)(ktLights->lights[i]);
        lights[i] = *light;
    }

    LightingUBO lightingUBO = {0};
    lightingUBO.numLightsInUse = ktLights->size;
    memcpy(lightingUBO.lights, lights, sizeof(Light) * lightingUBO.numLightsInUse);
    glm_vec3_copy(camera->position, lightingUBO.cameraPos);
    glm_vec3_copy((vec3) {0.04f, 0.04f, 0.04f}, lightingUBO.ambientLightColor);

    void *lightsData;
    vkMapMemory(device, renderObject->lightingUBO->memory, 0, sizeof(LightingUBO), 0, &lightsData);
    memcpy(lightsData, &lightingUBO, sizeof(LightingUBO));
    vkUnmapMemory(device, renderObject->lightingUBO->memory);

    free(ktLights->lights);
    free(ktLights);
}

void updateTransformUBO(VkDevice device, RenderObject *renderObject, Camera *camera) {
    TransformUBO transformUBO = {0};
    glm_mat4_identity(transformUBO.model);
    glm_scale(transformUBO.model, renderObject->scale);
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[0]), (vec3) {1.0f, 0.0f, 0.0f}); // X rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[1]), (vec3) {0.0f, 1.0f, 0.0f}); // Y rotation
    glm_rotate(transformUBO.model, glm_rad(renderObject->rotation[2]), (vec3) {0.0f, 0.0f, 1.0f}); // Z rotation
    glm_translate(transformUBO.model, renderObject->position);

    memcpy(transformUBO.view, camera->view, sizeof(mat4));
    memcpy(transformUBO.proj, camera->proj, sizeof(mat4));

    void *transformData;
    vkMapMemory(device, renderObject->transformUBO->memory, 0, sizeof(TransformUBO), 0, &transformData);
    memcpy(transformData, &transformUBO, sizeof(TransformUBO));
    vkUnmapMemory(device, renderObject->transformUBO->memory);
}
