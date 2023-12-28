#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include "kotlin-game/cinterop/model.h"

typedef struct {
    float position[3];
    float normal[3];
    float uv[2];
    float tangent[4];
} Vertex;

VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);
VkPipeline createPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkRenderPass renderPass, Viewport viewport, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
void destroyPipelineLayout(VkDevice device, VkPipelineLayout pipelineLayout);
void destroyPipeline(VkDevice device, VkPipeline pipeline);
#endif // VULKAN_PIPELINE_H