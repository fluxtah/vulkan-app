package com.fluxtah.application.api.camera

import com.fluxtah.application.api.interop.*
import kotlinx.cinterop.ExperimentalForeignApi

@OptIn(ExperimentalForeignApi::class)
class Camera(val handle: CCamera, val behaviors: List<CameraBehavior>) {

    init {
        behaviors.forEach { it.camera = this }
    }

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

    fun resetBehaviors() {
        behaviors.forEach { it.reset() }
    }
}
