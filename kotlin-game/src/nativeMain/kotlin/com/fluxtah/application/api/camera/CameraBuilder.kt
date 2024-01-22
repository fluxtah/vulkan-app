package com.fluxtah.application.api.camera

import com.fluxtah.application.api.interop.c_createCamera
import com.fluxtah.application.api.interop.model.CreateCameraInfo
import com.fluxtah.application.api.scene.Scene
import com.fluxtah.application.api.scene.SceneDsl
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.memScoped

@SceneDsl
class CameraBuilder(private val scene: Scene) {
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var fieldOfView: Float = 45.0f
    private var nearPlane: Float = 0.1f
    private var farPlane: Float = 100.0f
    private var aspectRatio: Float = 0.0f

    private val behaviors = mutableListOf<() -> CameraBehavior>()

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

    fun behavior(behavior: () -> CameraBehavior) {
        behaviors.add(behavior)
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): Camera {

        val cCamera = memScoped {
            val info = cValue<CreateCameraInfo> {
                this.positionX = this@CameraBuilder.positionX
                this.positionY = this@CameraBuilder.positionY
                this.positionZ = this@CameraBuilder.positionZ
                this.fov = this@CameraBuilder.fieldOfView
                this.near = this@CameraBuilder.nearPlane
                this.far = this@CameraBuilder.farPlane
                this.aspect = this@CameraBuilder.aspectRatio
            }
            c_createCamera!!.invoke(info.ptr)
        }

        val behaviors = behaviors.map { b -> b().apply { this.scene = this@CameraBuilder.scene } }

        return Camera(cCamera, behaviors)
    }
}