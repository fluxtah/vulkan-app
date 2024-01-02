package com.fluxtah.application.api

import com.fluxtah.application.MyApplication
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.interop.c_isKeyPressed

interface Application {
    fun initialize()
    fun update(time: Float, deltaTime: Float) {}
}

fun isKeyPressed(key: Key): Boolean {
    return c_isKeyPressed?.invoke(key.value) == 1
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
    val activeSceneInfo = activeScene
    if (activeSceneInfo != null) {
        activeSceneInfo.onSceneUpdate?.invoke(activeSceneInfo.scene, time, deltaTime)
    }
    applicationInstance.update(time, deltaTime)
}
