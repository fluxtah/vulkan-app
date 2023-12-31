package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CEntity = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CreateEntityFunc = (CVulkanContext, String) -> CEntity

@OptIn(ExperimentalForeignApi::class)
var c_createEntity: CreateEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateEntityFunc")
fun ktSetCreateEntityFunc(callback: CPointer<CFunction<(CVulkanContext, CPointer<ByteVar>) -> CEntity>>) {
    c_createEntity = { context, name ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CPointer<ByteVar>) -> CEntity>>()(
                context,
                name.cstr.ptr
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyEntityFunc = (CVulkanContext, CEntity) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroyEntity: DestroyEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyEntityFunc")
fun ktSetDestroyEntityFunc(callback: CPointer<CFunction<(CVulkanContext, CEntity) -> Unit>>) {
    c_destroyEntity = { device, renderObject ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CEntity) -> Unit>>()(device, renderObject)
        }
    }
}