#ifndef APP_PIPELINES_SHADER_UTIL_H
#define APP_PIPELINES_SHADER_UTIL_H

#include <vulkan/vulkan.h>
#include <stdlib.h>

VkPipelineShaderStageCreateInfo *createBasicShaderStages(
        VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
#endif //APP_PIPELINES_SHADER_UTIL_H
