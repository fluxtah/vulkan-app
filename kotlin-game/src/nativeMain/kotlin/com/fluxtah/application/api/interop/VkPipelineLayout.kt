package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkPipelineLayout = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CreatePipelineLayoutFunc = (VkDevice, VkDescriptorSetLayout) -> VkPipelineLayout

@OptIn(ExperimentalForeignApi::class)
var createPipelineLayoutFuncCallback: CreatePipelineLayoutFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreatePipelineLayoutFunc")
fun ktSetCreatePipelineLayoutFunc(callback: CPointer<CFunction<(VkDevice, VkDescriptorSetLayout) -> VkPipelineLayout>>) {
    createPipelineLayoutFuncCallback = { device, descriptorSet ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, VkDescriptorSetLayout) -> VkPipelineLayout>>()(device, descriptorSet)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyPipelineLayoutFunc = (VkDevice, VkPipelineLayout) -> Unit

@OptIn(ExperimentalForeignApi::class)
var destroyPipelineLayoutFuncCallback: DestroyPipelineLayoutFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyPipelineLayoutFunc")
fun ktSetDestroyPipelineLayoutFunc(callback: CPointer<CFunction<(VkDevice, VkPipelineLayout) -> Unit>>) {
    destroyPipelineLayoutFuncCallback = { device, pipelineLayout ->
        memScoped {
            callback.reinterpret<CFunction<(VkDevice, VkPipelineLayout) -> Unit>>()(device, pipelineLayout)
        }
    }
}