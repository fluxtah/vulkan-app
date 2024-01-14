package com.fluxtah.application.api

import com.fluxtah.application.api.interop.*
import com.fluxtah.application.api.interop.model.CreateCameraInfo
import com.fluxtah.application.api.scene.SceneDsl
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.memScoped

@OptIn(ExperimentalForeignApi::class)
class Camera(val handle: CCamera) {
    fun moveForward(amount: Float) {
        c_moveCameraForward!!.invoke(handle, amount)
    }

    fun moveBackward(amount: Float) {
        c_moveCameraBackward!!.invoke(handle, amount)
    }

    fun moveLeft(amount: Float) {
        c_moveCameraLeft!!.invoke(handle, amount)
    }

    fun moveRight(amount: Float) {
        c_moveCameraRight!!.invoke(handle, amount)
    }

    /**
     * Adjusts the camera's pitch.
     * @param amount Positive value to pitch up, negative to pitch down.
     */
    fun pitch(amount: Float) {
        c_pitchCamera!!.invoke(handle, amount)
    }

    /**
     * Adjusts the camera's yaw.
     * @param amount Positive value to yaw right, negative to yaw left.
     */
    fun yaw(amount: Float) {
        c_yawCamera!!.invoke(handle, amount)
    }

    fun lookAt(x: Float, y: Float, z: Float) {
        c_setCameraLookAt!!.invoke(handle, x, y, z)
    }

    fun position(x: Float, y: Float, z: Float) {
        c_positionCamera!!.invoke(handle, x, y, z)
    }

    fun applyChanges() {
        c_applyCameraChanges!!.invoke(handle)
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
    private var aspectRatio: Float = 0.0f
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
            this.positionX = this@CameraBuilder.positionX
            this.positionY = this@CameraBuilder.positionY
            this.positionZ = this@CameraBuilder.positionZ
            this.fov = this@CameraBuilder.fieldOfView
            this.near = this@CameraBuilder.nearPlane
            this.far = this@CameraBuilder.farPlane
            this.aspect = this@CameraBuilder.aspectRatio
        }

        val cCamera = memScoped {
            c_createCamera!!.invoke(info.ptr)
        }

        return Camera(cCamera)
    }
}
