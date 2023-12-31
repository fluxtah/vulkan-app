package com.fluxtah.application.api

class Camera

@SceneDsl
class CameraBuilder {
    fun position(x: Float, y: Float, z: Float) { /* ... */
    }

    fun build(): Camera {
        return Camera()
    }
}
