package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkRenderPass = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private val vkRenderPasses = mutableMapOf<String, VkRenderPass>()

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkRenderPass")
fun ktGetVkRenderPass(name: String): VkRenderPass? {
    return vkRenderPasses[name]
}

@OptIn(ExperimentalForeignApi::class)
fun registerRenderPass(name: String, renderPass: VkRenderPass) {
    vkRenderPasses[name] = renderPass
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktDestroyAllRenderPasses")
fun ktDestroyAllRenderPasses() {
    vkRenderPasses.values.forEach {
        destroyRenderPassFuncCallback?.invoke(ktGetVulkanContext(), it)
    }
    vkRenderPasses.clear()
}

@OptIn(ExperimentalForeignApi::class)
typealias CreateRenderPassFunc = (VulkanContext) -> VkRenderPass

@OptIn(ExperimentalForeignApi::class)
var createRenderPassFuncCallback: CreateRenderPassFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateRenderPassFunc")
fun ktSetCreateRenderPassFunc(callback: CPointer<CFunction<((VulkanContext)) -> VkRenderPass>>) {
    createRenderPassFuncCallback = { context ->
        memScoped {
            callback.reinterpret<CFunction<((VulkanContext)) -> VkRenderPass>>()(context)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyRenderPassFunc = (VulkanContext, VkRenderPass) -> Unit

@OptIn(ExperimentalForeignApi::class)
var destroyRenderPassFuncCallback: DestroyRenderPassFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyRenderPassFunc")
fun ktSetDestroyRenderPassFunc(callback: CPointer<CFunction<(VulkanContext, VkRenderPass) -> Unit>>) {
    destroyRenderPassFuncCallback = { context, renderPass ->
        memScoped {
            callback.reinterpret<CFunction<(VulkanContext, VkRenderPass) -> Unit>>()(context, renderPass)
        }
    }
}

