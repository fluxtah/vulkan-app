package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CEntity = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CreateEntityFunc = (VulkanContext, String) -> CEntity

@OptIn(ExperimentalForeignApi::class)
var c_createEntity: CreateEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateEntityFunc")
fun ktSetCreateEntityFunc(callback: CPointer<CFunction<(VulkanContext, CPointer<ByteVar>) -> CEntity>>) {
    c_createEntity = { context, name ->
        memScoped {
            callback.reinterpret<CFunction<(VulkanContext, CPointer<ByteVar>) -> CEntity>>()(
                context,
                name.cstr.ptr
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyEntityFunc = (VulkanContext, CEntity) -> Unit

@OptIn(ExperimentalForeignApi::class)
var destroyEntityFuncCallback: DestroyEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyEntityFunc")
fun ktSetDestroyEntityFunc(callback: CPointer<CFunction<(VulkanContext, CEntity) -> Unit>>) {
    destroyEntityFuncCallback = { device, renderObject ->
        memScoped {
            callback.reinterpret<CFunction<(VulkanContext, CEntity) -> Unit>>()(device, renderObject)
        }
    }
}