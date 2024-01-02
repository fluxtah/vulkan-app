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

@OptIn(ExperimentalForeignApi::class)
typealias MoveCameraLeftFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_moveCameraLeft: MoveCameraLeftFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetMoveCameraLeftFunc")
fun ktSetMoveCameraLeftFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_moveCameraLeft = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias MoveCameraRightFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_moveCameraRight: MoveCameraRightFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetMoveCameraRightFunc")
fun ktSetMoveCameraRightFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_moveCameraRight = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PitchCameraUpFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_pitchCameraUp: PitchCameraUpFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPitchCameraUpFunc")
fun ktSetPitchCameraUpFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_pitchCameraUp = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PitchCameraDownFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_pitchCameraDown: PitchCameraDownFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPitchCameraDownFunc")
fun ktSetPitchCameraDownFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_pitchCameraDown = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias YawCameraLeftFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_yawCameraLeft: YawCameraLeftFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetYawCameraLeftFunc")
fun ktSetYawCameraLeftFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_yawCameraLeft = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias YawCameraRightFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_yawCameraRight: YawCameraRightFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetYawCameraRightFunc")
fun ktSetYawCameraRightFunc(callback: CPointer<CFunction<(CCamera, Float) -> Unit>>) {
    c_yawCameraRight = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera, Float) -> Unit>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias ApplyCameraChangesFunc = (CCamera) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_applyCameraChanges: ApplyCameraChangesFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetApplyCameraChangesFunc")
fun ktSetApplyCameraChangesFunc(callback: CPointer<CFunction<(CCamera) -> Unit>>) {
    c_applyCameraChanges = { camera ->
        memScoped {
            callback.reinterpret<CFunction<(CCamera) -> Unit>>()(camera)
        }
    }
}