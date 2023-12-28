package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkPipeline = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
data class PipelineCacheEntry(val pipeline: VkPipeline, val pipelineLayout: VkPipelineLayout)

@OptIn(ExperimentalForeignApi::class)
private val vkPipelines = mutableMapOf<String, PipelineCacheEntry>()

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkPipeline")
fun ktGetVkPipeline(name: String): VkPipeline? {
    return vkPipelines[name]?.pipeline
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkPipelineLayout")
fun ktGetVkPipelineLayout(name: String): VkPipelineLayout? {
    return vkPipelines[name]?.pipelineLayout
}


@OptIn(ExperimentalForeignApi::class)
fun registerPipeline(name: String, pipeline: VkPipeline, pipelineLayout: VkPipelineLayout) {
    vkPipelines[name] = PipelineCacheEntry(pipeline, pipelineLayout)
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktDestroyAllPipelines")
fun ktDestroyAllPipelines() {
    vkPipelines.values.forEach {
        destroyPipelineFuncCallback?.invoke(ktGetVkDevice(), it.pipeline)
        destroyPipelineLayoutFuncCallback?.invoke(ktGetVkDevice(), it.pipelineLayout)
    }
    vkPipelines.clear()
}

@OptIn(ExperimentalForeignApi::class)
typealias CreatePipelineFunc = (VkDevice, VkPipelineLayout, VkRenderPass, VkViewport, VkShaderModule, VkShaderModule) -> VkPipeline

@OptIn(ExperimentalForeignApi::class)
var createPipelineFuncCallback: CreatePipelineFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreatePipelineFunc")
fun ktSetCreatePipelineFunc(callback: CPointer<CFunction<(VkDevice, VkPipelineLayout, VkRenderPass, VkViewport, VkShaderModule, VkShaderModule) -> VkPipeline>>) {
    createPipelineFuncCallback = { device, pipeline, renderPass, viewport, vertShader, fragShader ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, VkPipelineLayout, VkRenderPass, VkViewport, VkShaderModule, VkShaderModule) -> VkPipeline>>()(
                device, pipeline, renderPass, viewport, vertShader, fragShader
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyPipelineFunc = (VkDevice, VkPipeline) -> Unit

@OptIn(ExperimentalForeignApi::class)
var destroyPipelineFuncCallback: DestroyPipelineFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyPipelineFunc")
fun ktSetDestroyPipelineFunc(callback: CPointer<CFunction<(VkDevice, VkPipeline) -> Unit>>) {
    destroyPipelineFuncCallback = { device, pipeline ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, VkPipeline) -> Unit>>()(device, pipeline)
        }
    }
}