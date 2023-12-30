#ifndef APP_RENDEROBJECT_H
#define APP_RENDEROBJECT_H

#include "include/app_modeldata.h"
#include "include/app_ubo.h"
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
    BufferMemory *lightArrayUBO;
    VkDescriptorSet vertexDescriptorSet;
    VkDescriptorSet fragmentDescriptorSet;

} RenderObject;

RenderObject *createRenderObjectFromFile(
        VulkanContext *context,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout,
        VkDescriptorPool descriptorPool,
        const char *filename);
void setupTextureFromImageData(VulkanContext *context, ModelImageData *imageData, ImageMemory *imageMemory);

void destroyRenderObject(VulkanContext *context, RenderObject *obj);
#endif //APP_RENDEROBJECT_H
