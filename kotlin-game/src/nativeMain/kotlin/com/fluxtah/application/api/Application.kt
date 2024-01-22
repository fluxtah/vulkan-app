package com.fluxtah.application.api

import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.interop.*
import com.fluxtah.application.api.scene.SceneImpl
import com.fluxtah.application.api.scene.activeSceneInfo
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

@OptIn(ExperimentalForeignApi::class)
fun enableDebugBoundingVolumes(enable: Boolean) {
    c_setEnableDebugBoundingVolumes?.invoke(ApplicationContext.vulcanContext!!, enable)
}

private lateinit var applicationInstance: Application

@OptIn(ExperimentalNativeApi::class)
@CName("ktCreateApplication")
fun ktCreateApplication() {
    applicationInstance = ShipGame()
}

@OptIn(ExperimentalNativeApi::class)
@CName("ktInitApplication")
fun ktInitApplication() {
    applicationInstance.initialize()
}

const val fixedTimeStep = 1.0f / 60.0f // Fixed timestep (e.g., 60 updates per second)
private var accumulatedTime = 0.0f

@OptIn(ExperimentalNativeApi::class)
@CName("ktUpdateApplication")
fun ktUpdateApplication(time: Float, deltaTime: Float) {
    accumulatedTime += deltaTime
    val activeSceneInfo = activeSceneInfo
    val scene = activeSceneInfo.scene as SceneImpl
    val entities = scene.entities.filter { it.value.entity.inUse }.map { it.value } + scene.entityPools.flatMap { it.value.entitiesInUse }
    val emitters = scene.emitters.map { it.value } + scene.emitterPools.flatMap { it.value.emittersInUse }

    applicationInstance.beforeUpdate(time, deltaTime)

    activeSceneInfo.onSceneBeforeUpdate?.invoke(activeSceneInfo.scene, time, deltaTime)

    scene.sequencesPlaying.forEach { sequence ->
        sequence.advance(time, deltaTime)
    }

    entities.forEach {
        it.behaviors.forEach { behavior ->
            behavior.beforeUpdate(time, deltaTime)
        }
        it.onSceneBeforeEntityUpdate?.invoke(scene, it.entity, time, deltaTime)
    }
    emitters.forEach {
        it.behaviors.forEach { behavior ->
            behavior.beforeUpdate(time, deltaTime)
        }
    }

    while (accumulatedTime >= fixedTimeStep) {
        activeSceneInfo.onSceneUpdate?.invoke(activeSceneInfo.scene, time)
        entities.forEach {
            it.behaviors.forEach { behavior ->
                behavior.update(time)
            }
            it.onSceneEntityUpdate?.invoke(scene, it.entity, time)
        }
        emitters.forEach {
            it.behaviors.forEach { behavior ->
                behavior.update(time)
            }
        }
        applicationInstance.update(time)
        accumulatedTime -= fixedTimeStep
    }

    activeSceneInfo.onSceneAfterUpdate?.invoke(activeSceneInfo.scene, time, deltaTime)
    entities.forEach {
        it.behaviors.forEach { behavior ->
            behavior.afterUpdate(time, deltaTime)
        }
        it.onSceneAfterEntityUpdate?.invoke(scene, it.entity, time, deltaTime)
    }
    emitters.forEach {
        it.behaviors.forEach { behavior ->
            behavior.afterUpdate(time, deltaTime)
        }
    }

    applicationInstance.afterUpdate(time, deltaTime)
}

@OptIn(ExperimentalNativeApi::class)
@CName("ktDestroyApplication")
fun ktDestroyApplication() {
    // TODO: Destroy all scenes
    (activeSceneInfo.scene as SceneImpl).destroy()
}

@OptIn(ExperimentalForeignApi::class)
private fun SceneImpl.destroy() {
    lights.forEach { light ->
        c_destroyLight!!.invoke(light.value.handle)
    }
    lights.clear()

    cameras.forEach { camera ->
        c_destroyCamera!!.invoke(camera.value.handle)
    }
    cameras.clear()

    entities.forEach { entityInfo ->
        c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, entityInfo.value.entity.handle)
        entityInfo.value.stableRef!!.dispose()
    }
    entities.clear()
    entityPools.forEach { entityPool ->
        entityPool.value.entitiesInUse.forEach { entityInfo ->
            c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, entityInfo.entity.handle)
            entityInfo.stableRef!!.dispose()
        }
        entityPool.value.entitiesInUse.clear()
        entityPool.value.entitiesAvailable.forEach { entityInfo ->
            c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, entityInfo.entity.handle)
            entityInfo.stableRef!!.dispose()
        }
        entityPool.value.entitiesAvailable.clear()
    }

    emitters.forEach { emitterInfo ->
        c_destroyEmitter!!.invoke(ApplicationContext.vulcanContext!!, emitterInfo.value.emitter.handle)
    }
    emitters.clear()
    emitterPools.forEach { emitterPool ->
        emitterPool.value.emittersInUse.forEach { emitterInfo ->
            c_destroyEmitter!!.invoke(ApplicationContext.vulcanContext!!, emitterInfo.emitter.handle)
        }
        emitterPool.value.emittersInUse.clear()
        emitterPool.value.emittersAvailable.forEach { emitterInfo ->
            c_destroyEmitter!!.invoke(ApplicationContext.vulcanContext!!, emitterInfo.emitter.handle)
        }
        emitterPool.value.emittersAvailable.clear()
    }

    sounds.forEach {
        c_destroySound!!.invoke(it.value.handle)
    }
    sounds.clear()
}
