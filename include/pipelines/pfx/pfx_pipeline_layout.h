#ifndef APP_PIPELINES_PFX_LAYOUT_H
#define APP_PIPELINES_PFX_LAYOUT_H
#include <vulkan/vulkan.h>
#include "include/debug.h"

VkPipelineLayout createPfxPipelineLayout(
        VkDevice device,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout);

#endif //APP_PIPELINES_PFX_LAYOUT_H
