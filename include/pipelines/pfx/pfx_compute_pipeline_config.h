#ifndef APP_PIPELINES_PFX_COMPUTE_PIPELINE_CONFIG_H
#define APP_PIPELINES_PFX_COMPUTE_PIPELINE_CONFIG_H

#include <vulkan/vulkan.h>
#include <stdlib.h>
#include "include/pipelines/compute_pipeline_config.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_layout.h"
#include "include/pipelines/pfx/pfx_compute_pipeline.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_descriptor_pool.h"
#include "include/pipelines/pfx/pfx_compute_pipeline_descriptor_set_layout.h"
#include "include/vulkan_device_context.h"
#include "include/application_context.h"
#include "include/vulkan/commandbuffer.h"

ComputePipelineConfig *createPfxComputePipelineConfig(
        VulkanDeviceContext *context,
        VkCommandPool commandPool,
        int maxParticles);

void recordComputeCommandBuffer(EmitterArray *emitters, float deltaTime);

#endif //APP_PIPELINES_PFX_COMPUTE_PIPELINE_CONFIG_H
