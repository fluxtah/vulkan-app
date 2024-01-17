#ifndef APP_PIPELINES_PFX_UPDATE_DESCRIPTOR_SETS_H
#define APP_PIPELINES_PFX_UPDATE_DESCRIPTOR_SETS_H

#include <vulkan/vulkan.h>
#include "include/debug.h"
#include "include/ubo.h"

void updatePfxPipelineDescriptorSets(
        VkDevice device,
        VkDescriptorSet vertexDescriptorSet,
        VkDescriptorSet fragmentDescriptorSet,
        VkBuffer transformUboBuffer,
        VkImageView baseColorImageView,
        VkSampler sampler);

#endif //APP_PIPELINES_PFX_UPDATE_DESCRIPTOR_SETS_H
