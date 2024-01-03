#ifndef APP_RENDEROBJECT_H
#define APP_RENDEROBJECT_H

#include "include/modeldata.h"
#include "include/ubo.h"
#include "include/vulkan/buffer.h"
#include "include/vulkan/context.h"
#include "include/vulkan/descriptor.h"
#include "include/vulkan/image.h"

#include <cglm/cglm.h>
#include <vulkan/vulkan.h>

typedef struct {
    VkImage image;
    VkDeviceMemory memory;
    VkImageView imageView;
} ImageMemory;

typedef struct {
    vec3 scale;
    vec3 position;
    vec3 rotation;

    ModelData *modelData;
    BufferMemory *vertexBuffer;
    BufferMemory *indexBuffer;

    ImageMemory *colorMap;
    ImageMemory *normalMap;
    ImageMemory *metallicRoughnessMap;

    BufferMemory *transformUBO;
    BufferMemory *lightingUBO;
    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;

} RenderObject;

RenderObject *createRenderObjectFromFile(VulkanContext *context, const char *filename, CreateEntityInfo *info);
void setupTextureFromImageData(VulkanContext *context, ModelImageData *imageData, ImageMemory *imageMemory);

void rotateRenderObject(RenderObject *obj, float x, float y, float z);

void destroyRenderObject(VulkanContext *context, RenderObject *obj);
#endif //APP_RENDEROBJECT_H
