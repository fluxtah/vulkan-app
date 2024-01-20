#ifndef APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H
#define APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H

#include <vulkan/vulkan.h>
#include "include/debug.h"
#include <cglm/mat4.h>

typedef struct PfxComputePipelinePushConstants {
    mat4 model;
    float deltaTime;
    int reset;
} PfxComputePipelinePushConstants;

VkPipelineLayout createPfxComputePipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout);
#endif //APP_PIPELINES_PFX_COMPUTE_PIPELINE_LAYOUT_H
