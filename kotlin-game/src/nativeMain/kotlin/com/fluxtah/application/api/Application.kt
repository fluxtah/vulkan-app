package com.fluxtah.application.api

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

private const val fixedTimeStep = 1.0f / 60.0f // Fixed timestep (e.g., 60 updates per second)
private var accumulatedTime = 0.0f

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktUpdateApplication")
fun ktUpdateApplication(time: Float, deltaTime: Float) {
    val activeSceneInfo = activeScene
    accumulatedTime += deltaTime

    while (accumulatedTime >= fixedTimeStep) {
        if (activeSceneInfo != null) {
            activeSceneInfo.onSceneUpdate?.invoke(activeSceneInfo.scene, time, fixedTimeStep)
            val scene = activeSceneInfo.scene
            scene.entities.values.forEach {
                it.behaviors.forEach { behavior ->
                    behavior.update(scene, it.entity, time, fixedTimeStep)
                }
                it.onSceneEntityUpdate?.invoke(scene, it.entity, time, fixedTimeStep)
            }
        }
        applicationInstance.update(time, fixedTimeStep)
        accumulatedTime -= fixedTimeStep
    }
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
