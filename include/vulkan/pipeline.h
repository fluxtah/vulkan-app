#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include "kotlin-game/cinterop/model.h"
#include "include/debug.h"

typedef struct {
    float position[3];
    float normal[3];
    float uv[2];
    float tangent[4];
} Vertex;

typedef struct {
    float position[3];
} DebugVertex;


VkPipelineLayout createBasicPipelineLayout(
        VkDevice device,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout);

VkPipelineLayout createDebugPipelineLayout(VkDevice device);

VkPipeline createBasicPipeline(
        VkDevice device,
        VkPipelineLayout pipelineLayout,
        VkRenderPass renderPass,
        Viewport viewport,
        VkShaderModule vertShaderModule,
        VkShaderModule fragShaderModule);

VkPipeline createDebugPipeline(
        VkDevice device,
        VkPipelineLayout pipelineLayout,
        VkRenderPass renderPass,
        Viewport viewport,
        VkShaderModule vertShaderModule,
        VkShaderModule fragShaderModule);

#endif // VULKAN_PIPELINE_H