#ifndef VULKAN_DESCRIPTOR_H
#define VULKAN_DESCRIPTOR_H

#include "include/ubo.h"
#include "include/debug.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

VkDescriptorPool createBasicShaderDescriptorPool(VkDevice device);

VkDescriptorSetLayout createBasicVertexShaderDescriptorSetLayout(VkDevice device);

VkDescriptorSetLayout createBasicFragmentShaderDescriptorSetLayout(VkDevice device);

VkResult allocateDescriptorSet(
        VkDevice device,
        VkDescriptorPool descriptorPool,
        VkDescriptorSetLayout layout,
        VkDescriptorSet *descriptorSet);

void updateBasicShaderDescriptorSet(
        VkDevice device,
        VkDescriptorSet vertexDescriptorSet,
        VkDescriptorSet fragmentDescriptorSet,
        VkBuffer transformUboBuffer,
        VkBuffer lightingUboBuffer,
        VkImageView baseColorImageView,
        VkImageView normalMapImageView,
        VkImageView metallicRoughnessMapImageView,
        VkSampler sampler);

#endif // VULKAN_DESCRIPTOR_H