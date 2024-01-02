package com.fluxtah.application.api

import com.fluxtah.application.api.interop.CCamera
import com.fluxtah.application.api.interop.c_createCamera
import com.fluxtah.application.api.interop.c_moveCameraBackward
import com.fluxtah.application.api.interop.c_moveCameraForward
import com.fluxtah.application.api.interop.model.CreateCameraInfo
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.memScoped

@OptIn(ExperimentalForeignApi::class)
class Camera(private val handle: CCamera) {
    fun moveForward(amount: Float) {
        c_moveCameraForward!!.invoke(handle, amount)
    }

    fun moveBackward(amount: Float) {
        c_moveCameraBackward!!.invoke(handle, amount)
    }

    fun moveLeft(amount: Float) {
        TODO("Not yet implemented")
    }

    fun moveRight(camera: Camera, amount: Float) {
        TODO("Not yet implemented")
    }

    fun pitchUp(camera: Camera, amount: Float) {
        TODO("Not yet implemented")
    }

    fun pitchDown(camera: Camera, amount: Float) {
        TODO("Not yet implemented")
    }

    fun yawLeft(camera: Camera, amount: Float) {
        TODO("Not yet implemented")
    }

    fun yawRight(camera: Camera, amount: Float) {
        TODO("Not yet implemented")
    }

    fun applyChanges() {
        TODO("Not yet implemented")
    }
}

@SceneDsl
class CameraBuilder {
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var fieldOfView: Float = 45.0f
    private var nearPlane: Float = 0.1f
    private var farPlane: Float = 100.0f
    private var aspectRatio: Float = 1.0f
    fun position(x: Float, y: Float, z: Float) {
        positionX = x
        positionY = y
        positionZ = z
    }

    fun fieldOfView(fov: Float) {
        fieldOfView = fov
    }

    fun nearPlane(near: Float) {
        nearPlane = near
    }

    fun farPlane(far: Float) {
        farPlane = far
    }

    fun aspectRatio(aspect: Float) {
        aspectRatio = aspect
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): Camera {
        val info = cValue<CreateCameraInfo> {
            positionX = this@CameraBuilder.positionX
            positionY = this@CameraBuilder.positionY
            positionZ = this@CameraBuilder.positionZ
            fov = this@CameraBuilder.fieldOfView
            near = this@CameraBuilder.nearPlane
            far = this@CameraBuilder.farPlane
            aspect = this@CameraBuilder.aspectRatio
        }

        val cCamera = memScoped {
            c_createCamera!!.invoke(ApplicationContext.vulcanContext!!, info.ptr)
        }

        return Camera(cCamera)
    }
}
