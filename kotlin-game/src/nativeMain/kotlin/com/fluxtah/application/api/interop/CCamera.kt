package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.CreateCameraInfo
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CCamera = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CCreateCameraInfo = CPointer<CreateCameraInfo>

@OptIn(ExperimentalForeignApi::class)
typealias CreateCameraFunc = (CCreateCameraInfo) -> CCamera

@OptIn(ExperimentalForeignApi::class)
var c_createCamera: CreateCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateCameraFunc")
fun ktSetCreateCameraFunc(callback: CPointer<CFunction<CreateCameraFunc>>) {
    c_createCamera = { info ->
        memScoped {
            callback.reinterpret<CFunction<CreateCameraFunc>>()(info)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyCameraFunc = (CCamera) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroyCamera: DestroyCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyCameraFunc")
fun ktSetDestroyCameraFunc(callback: CPointer<CFunction<DestroyCameraFunc>>) {
    c_destroyCamera = { camera ->
        memScoped {
            callback.reinterpret<CFunction<DestroyCameraFunc>>()(camera)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias MoveCameraForwardFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_moveCameraForward: MoveCameraForwardFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetMoveCameraForwardFunc")
fun ktSetMoveCameraForwardFunc(callback: CPointer<CFunction<MoveCameraForwardFunc>>) {
    c_moveCameraForward = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<MoveCameraForwardFunc>>()(
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
fun ktSetMoveCameraBackwardFunc(callback: CPointer<CFunction<MoveCameraBackwardFunc>>) {
    c_moveCameraBackward = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<MoveCameraBackwardFunc>>()(
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
fun ktSetMoveCameraLeftFunc(callback: CPointer<CFunction<MoveCameraLeftFunc>>) {
    c_moveCameraLeft = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<MoveCameraLeftFunc>>()(
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
fun ktSetMoveCameraRightFunc(callback: CPointer<CFunction<MoveCameraRightFunc>>) {
    c_moveCameraRight = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<MoveCameraRightFunc>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PitchCameraFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_pitchCamera: PitchCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPitchCameraFunc")
fun ktSetPitchCameraFunc(callback: CPointer<CFunction<PitchCameraFunc>>) {
    c_pitchCamera = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<PitchCameraFunc>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias YawCameraFunc = (CCamera, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_yawCamera: YawCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetYawCameraFunc")
fun ktSetYawCameraFunc(callback: CPointer<CFunction<YawCameraFunc>>) {
    c_yawCamera = { camera, amount ->
        memScoped {
            callback.reinterpret<CFunction<YawCameraFunc>>()(
                camera, amount
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias SetCameraLookAtFunc = (CCamera, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setCameraLookAt: SetCameraLookAtFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCameraLookAtFunc")
fun ktSetCameraLookAtFunc(callback: CPointer<CFunction<SetCameraLookAtFunc>>) {
    c_setCameraLookAt = { camera, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<SetCameraLookAtFunc>>()(
                camera, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PositionCameraFunc = (CCamera, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_positionCamera: PositionCameraFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPositionCameraFunc")
fun ktSetPositionCameraFunc(callback: CPointer<CFunction<PositionCameraFunc>>) {
    c_positionCamera = { camera, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<PositionCameraFunc>>()(
                camera, x, y, z
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
fun ktSetApplyCameraChangesFunc(callback: CPointer<CFunction<ApplyCameraChangesFunc>>) {
    c_applyCameraChanges = { camera ->
        memScoped {
            callback.reinterpret<CFunction<ApplyCameraChangesFunc>>()(camera)
        }
    }
}
