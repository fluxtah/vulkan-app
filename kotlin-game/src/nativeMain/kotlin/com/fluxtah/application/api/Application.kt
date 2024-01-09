package com.fluxtah.application.api

import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.interop.*
import com.fluxtah.application.apps.shipgame.ShipGame
import kotlinx.cinterop.ExperimentalForeignApi
import kotlin.experimental.ExperimentalNativeApi

interface Application {
    fun initialize()
    fun beforeUpdate(time: Float, deltaTime: Float) {}
    fun update(time: Float) {}
    fun afterUpdate(time: Float, deltaTime: Float) {}
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

const val fixedTimeStep = 1.0f / 60.0f // Fixed timestep (e.g., 60 updates per second)
private var accumulatedTime = 0.0f

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktUpdateApplication")
fun ktUpdateApplication(time: Float, deltaTime: Float) {
    accumulatedTime += deltaTime
    val activeSceneInfo = activeScene
    val scene = activeSceneInfo.scene
    val entities = (scene as SceneImpl).entities

    activeSceneInfo.onSceneBeforeUpdate?.invoke(activeSceneInfo.scene, time, deltaTime)
    scene.entities.values.forEach {
        it.behaviors.forEach { behavior ->
            behavior.beforeUpdate(scene, it.entity, time, deltaTime)
        }
        it.onSceneBeforeEntityUpdate?.invoke(scene, it.entity, time, deltaTime)
    }

    applicationInstance.beforeUpdate(time, deltaTime)

    while (accumulatedTime >= fixedTimeStep) {
        activeSceneInfo.onSceneUpdate?.invoke(activeSceneInfo.scene, time)
        scene.entities.values.forEach {
            it.behaviors.forEach { behavior ->
                behavior.update(scene, it.entity, time)
            }
            it.onSceneEntityUpdate?.invoke(scene, it.entity, time)
        }
        applicationInstance.update(time)
        accumulatedTime -= fixedTimeStep
    }

    activeSceneInfo.onSceneAfterUpdate?.invoke(activeSceneInfo.scene, time, deltaTime)
    scene.entities.values.forEach {
        it.behaviors.forEach { behavior ->
            behavior.afterUpdate(scene, it.entity, time, deltaTime)
        }
        it.onSceneAfterEntityUpdate?.invoke(scene, it.entity, time, deltaTime)
    }

    applicationInstance.afterUpdate(time, deltaTime)
}

@OptIn(ExperimentalNativeApi::class, ExperimentalForeignApi::class)
@CName("ktDestroyApplication")
fun ktDestroyApplication() {
    // TODO: Destroy all scenes
    (activeScene.scene as SceneImpl).destroy()
}

@OptIn(ExperimentalForeignApi::class)
private fun SceneImpl.destroy() {
    cameras.forEach { camera ->
        c_destroyCamera!!.invoke(camera.value.handle)
    }
    cameras.clear()
    lights.forEach { light ->
        c_destroyLight!!.invoke(light.value.handle)
    }
    lights.clear()
    entities.forEach { entityInfo ->
        c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, entityInfo.value.entity.handle)
    }
    entities.clear()
    sounds.forEach {
        c_destroySound!!.invoke(it.value.handle)
    }
    sounds.clear()
}
