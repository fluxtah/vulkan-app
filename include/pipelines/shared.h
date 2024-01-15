#ifndef APP_PIPELINES_SHARED_H
#define APP_PIPELINES_SHARED_H

#include "include/context.h"
#include "include/vulkan/pipeline.h"

void destroyPipelineConfig(VulkanDeviceContext *context, PipelineConfig *pipelineConfig, uint32_t swapChainImageCount);


#endif //APP_PIPELINES_SHARED_H
