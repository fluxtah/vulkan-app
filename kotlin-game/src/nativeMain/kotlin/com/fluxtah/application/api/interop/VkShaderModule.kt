package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkShaderModule = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private val vkShaderModules = mutableMapOf<String, VkShaderModule>()

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkShaderModule")
fun ktGetVkShaderModule(name: String): VkShaderModule? {
    return vkShaderModules[name]
}

@OptIn(ExperimentalForeignApi::class)
fun registerShaderModule(name: String, module: VkShaderModule) {
    vkShaderModules[name] = module
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktDestroyAllShaderModules")
fun ktDestroyAllShaderModules() {
    vkShaderModules.values.forEach {
        destroyShaderModuleFuncCallback?.invoke(ktGetVkDevice(), it)
    }
    vkShaderModules.clear()
}

@OptIn(ExperimentalForeignApi::class)
typealias CreateShaderModuleFunc = (VkDevice, String) -> VkShaderModule

@OptIn(ExperimentalForeignApi::class)
var createShaderModuleFuncCallback: CreateShaderModuleFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateShaderModuleFunc")
fun ktSetCreateShaderModuleFunc(callback: CPointer<CFunction<(VkDevice, CPointer<ByteVar>) -> VkShaderModule>>) {
    createShaderModuleFuncCallback = { device, name ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, CPointer<ByteVar>) -> VkShaderModule>>()(device, name.cstr.ptr)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyShaderModuleFunc = (VkDevice, VkShaderModule) -> Unit

@OptIn(ExperimentalForeignApi::class)
var destroyShaderModuleFuncCallback: DestroyShaderModuleFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyShaderModuleFunc")
fun ktSetDestroyShaderModuleFunc(callback: CPointer<CFunction<(VkDevice, VkShaderModule) -> Unit>>) {
    destroyShaderModuleFuncCallback = { device, shaderModule ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, VkShaderModule) -> Unit>>()(device, shaderModule)
        }
    }
}