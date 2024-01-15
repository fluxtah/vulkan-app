#ifndef APP_PIPELINES_BASIC_VERTEX_DESC_SET_LAYOUT_H
#define APP_PIPELINES_BASIC_VERTEX_DESC_SET_LAYOUT_H

#include <vulkan/vulkan.h>
#include "include/debug.h"

VkDescriptorSetLayout createBasicPipelineVertexShaderDescriptorSetLayout(VkDevice device);

#endif //APP_PIPELINES_BASIC_VERTEX_DESC_SET_LAYOUT_H
