#ifndef APP_PIPELINES_BASIC_UPDATE_DESCRIPTOR_SETS_H
#define APP_PIPELINES_BASIC_UPDATE_DESCRIPTOR_SETS_H

#include <vulkan/vulkan.h>
#include "include/debug.h"
#include "include/ubo.h"

void updateBasicPipelineDescriptorSets(
        VkDevice device,
        VkDescriptorSet vertexDescriptorSet,
        VkDescriptorSet fragmentDescriptorSet,
        VkBuffer transformUboBuffer,
        VkBuffer lightingUboBuffer,
        VkImageView baseColorImageView,
        VkImageView normalMapImageView,
        VkImageView metallicRoughnessMapImageView,
        VkSampler sampler);

#endif //APP_PIPELINES_BASIC_UPDATE_DESCRIPTOR_SETS_H
