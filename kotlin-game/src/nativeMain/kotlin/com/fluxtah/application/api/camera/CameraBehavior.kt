package com.fluxtah.application.api.camera

import com.fluxtah.application.api.scene.Scene

abstract class CameraBehavior {
    lateinit var camera: Camera
    lateinit var scene: Scene
    open fun initialize() {}
    open fun beforeUpdate(time: Float, deltaTime: Float) {}
    open fun update(time: Float) {}
    open fun afterUpdate(time: Float, deltaTime: Float) {}
    open fun reset() {}
}
