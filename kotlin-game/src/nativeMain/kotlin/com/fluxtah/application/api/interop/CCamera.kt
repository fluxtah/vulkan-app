package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.CreateCameraInfo
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CCamera = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CCreateCameraInfo = CPointer<CreateCameraInfo>

@OptIn(ExperimentalForeignApi::class)
typealias CreateCameraFunc = (CVulkanContext, CCreateCameraInfo) -> CCamera

@OptIn(ExperimentalForeignApi::class)
var c_createCamera: CreateCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateCameraFunc")
fun ktSetCreateCameraFunc(callback: CPointer<CFunction<(CVulkanContext, CCreateCameraInfo) -> CCamera>>) {
    c_createCamera = { context, info ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CPointer<CreateCameraInfo>) -> CCamera>>()(
                context,
                info
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyCameraFunc = (CVulkanContext, CCamera) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroyCamera: DestroyCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyCameraFunc")
fun ktSetDestroyCameraFunc(callback: CPointer<CFunction<(CVulkanContext, CCamera) -> Unit>>) {
    c_destroyCamera = { device, camera ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CCamera) -> Unit>>()(device, camera)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias MoveCameraForwardFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_moveCameraForward: MoveCameraForwardFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetMoveCameraForwardFunc")
fun ktSetMoveCameraForwardFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_moveCameraForward = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias MoveCameraBackwardFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_moveCameraBackward: MoveCameraBackwardFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetMoveCameraBackwardFunc")
fun ktSetMoveCameraBackwardFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_moveCameraBackward = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}
