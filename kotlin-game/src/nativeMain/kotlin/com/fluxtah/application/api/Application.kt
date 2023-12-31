package com.fluxtah.application.api

import com.fluxtah.application.MyApplication

interface Application {
    fun initialize()
    fun update(time: Float, deltaTime: Float) {}
}

fun Application.setActiveCamera(id: String) {
    TODO("Not yet implemented")
}

class Light

@SceneDsl
class LightBuilder {
    fun color(r: Float, g: Float, b: Float, a: Float) { /* ... */
    }

    fun position(x: Float, y: Float, z: Float) { /* ... */
    }

    fun build(): Light {
        return Light()
    }
}

private lateinit var applicationInstance: Application

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktCreateApplication")
fun ktCreateApplication() {
    applicationInstance = MyApplication()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktInitApplication")
fun ktInitApplication() {
    applicationInstance.initialize()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktUpdateApplication")
fun ktUpdateApplication(time: Float, deltaTime: Float) {
    applicationInstance.update(time, deltaTime)
}
