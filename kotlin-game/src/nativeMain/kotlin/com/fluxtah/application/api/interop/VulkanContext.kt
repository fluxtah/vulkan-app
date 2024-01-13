package com.fluxtah.application.api.interop

import com.fluxtah.application.api.ApplicationContext
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CVulkanContext = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVulkanContext")
fun ktSetVulkanContext(context: CVulkanContext) {
    ApplicationContext.vulcanContext = context
}

@OptIn(ExperimentalForeignApi::class)
typealias SetActiveCameraFunc = (CVulkanContext, CCamera) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setActiveCamera: SetActiveCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetActiveCameraFunc")
fun ktSetActiveCameraFunc(callback: CPointer<CFunction<(CVulkanContext, CCamera) -> Unit>>) {
    c_setActiveCamera = { context, camera ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CCamera) -> Unit>>()(context, camera)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias SetEnableDebugBoundingVolumesFunc = (CVulkanContext, Boolean) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEnableDebugBoundingVolumes: SetEnableDebugBoundingVolumesFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetEnableDebugBoundingVolumesFunc")
fun ktSetEnableDebugBoundingVolumesFunc(callback: CPointer<CFunction<(CVulkanContext, Boolean) -> Unit>>) {
    c_setEnableDebugBoundingVolumes = { context, enable ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, Boolean) -> Unit>>()(context, enable)
        }
    }
}