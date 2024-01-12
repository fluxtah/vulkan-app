#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include "kotlin-game/cinterop/model.h"
#include "include/debug.h"
#include "include/vertex.h"

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