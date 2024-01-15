#ifndef APP_PIPELINES_BASIC_PIPELINE_LAYOUT_H
#define APP_PIPELINES_BASIC_PIPELINE_LAYOUT_H
#include <vulkan/vulkan.h>
#include "include/debug.h"

VkPipelineLayout createBasicPipelineLayout(
        VkDevice device,
        VkDescriptorSetLayout vertexDescriptorSetLayout,
        VkDescriptorSetLayout fragmentDescriptorSetLayout);

#endif //APP_PIPELINES_BASIC_PIPELINE_LAYOUT_H
