#ifndef VULKAN_DESCRIPTOR_H
#define VULKAN_DESCRIPTOR_H

#include "include/app_ubo.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

void createBasicShaderDescriptorPool(VkDevice device, VkDescriptorPool *descriptorPool);

void createBasicShaderDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout *descriptorSetLayout);

void allocateBasicShaderDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout,
                                      VkDescriptorSet *descriptorSet);

void updateBasicShaderDescriptorSet(VkDevice device, VkDescriptorSet descriptorSet, VkBuffer buffer,
                                    VkImageView baseColorImageView, VkImageView normalMapImageView, VkImageView metallicRoughnessMapImageView, VkSampler sampler);
#endif // VULKAN_DESCRIPTOR_H