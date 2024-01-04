package com.fluxtah.application.api

import com.fluxtah.application.MyApplication
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.interop.c_destroyCamera
import com.fluxtah.application.api.interop.c_destroyEntity
import com.fluxtah.application.api.interop.c_destroyLight
import com.fluxtah.application.api.interop.c_isKeyPressed
import com.fluxtah.application.apps.shipgame.ShipGame
import kotlinx.cinterop.ExperimentalForeignApi
import kotlin.experimental.ExperimentalNativeApi

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
    applicationInstance = ShipGame()
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

        activeSceneInfo.scene.entities.values.forEach {
            it.onSceneEntityUpdate?.invoke(activeSceneInfo.scene, it.entity, time, deltaTime)
        }
    }
    applicationInstance.update(time, deltaTime)
}

@OptIn(ExperimentalNativeApi::class, ExperimentalForeignApi::class)
@CName("ktDestroyApplication")
fun ktDestroyApplication() {
    val activeSceneInfo = activeScene
    activeSceneInfo?.scene?.cameras?.forEach {
        c_destroyCamera!!.invoke(it.value.handle)
    }
    activeSceneInfo?.scene?.cameras?.clear()
    activeSceneInfo?.scene?.lights?.forEach {
        c_destroyLight!!.invoke(it.value.handle)
    }
    activeSceneInfo?.scene?.lights?.clear()
    activeSceneInfo?.scene?.entities?.forEach {
        c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, it.value.entity.handle)
    }
    activeSceneInfo?.scene?.entities?.clear()

    // TODO destroy entities
}
