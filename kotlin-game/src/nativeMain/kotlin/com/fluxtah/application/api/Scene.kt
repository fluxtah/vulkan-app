package com.fluxtah.application.api

import com.fluxtah.application.api.interop.c_setActiveCamera
import kotlinx.cinterop.ExperimentalForeignApi

var activeScene: SceneInfo? = null
private val sceneBuilders = mutableMapOf<String, () -> SceneInfo>()
private val scenes = mutableMapOf<String, SceneInfo>()

data class SceneInfo(
    val scene: Scene,
    val onSceneCreated: ((Scene) -> Unit)?,
    val onSceneUpdate: OnSceneUpdate?,
    val onSceneBeforeUpdate: OnSceneBeforeUpdate?,
    val onSceneAfterUpdate: OnSceneAfterUpdate?
)

data class EntityInfo(
    val entity: Entity,
    val onSceneEntityUpdate: OnSceneEntityUpdate?,
    val onSceneBeforeEntityUpdate: OnSceneBeforeEntityUpdate?,
    val onSceneAfterEntityUpdate: OnSceneAfterEntityUpdate?,
    val behaviors: MutableList<EntityBehavior>
)

@DslMarker
annotation class SceneDsl

@OptIn(ExperimentalForeignApi::class)
class Scene {
    private var activeCamera: Camera? = null
    internal val cameras = mutableMapOf<String, Camera>()
    internal val lights = mutableMapOf<String, Light>()
    internal val entities = mutableMapOf<String, EntityInfo>()
    internal val sounds = mutableMapOf<String, Sound>()

    fun setActiveCamera(id: String) {
        activeCamera = cameras[id] ?: throw Exception("Camera with id $id does not exist")
        c_setActiveCamera!!.invoke(ApplicationContext.vulcanContext!!, activeCamera!!.handle)
    }

    fun activeCamera(): Camera? {
        return activeCamera
    }
}

typealias OnSceneUpdate = ((scene: Scene, time: Float) -> Unit)
typealias OnSceneBeforeUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneEntityUpdate = ((scene: Scene, entity: Entity, time: Float) -> Unit)
typealias OnSceneBeforeEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)

fun Application.scene(): Scene {
    return activeScene?.scene ?: throw Exception("No active scene")
}

fun Application.scene(id: String, block: SceneBuilder.() -> Unit) {
    sceneBuilders[id] = {
        val builder = SceneBuilder(id)
        block(builder)
        builder.build()
    }
}

@SceneDsl
class SceneBuilder(val sceneId: String) {
    private val entities = mutableMapOf<String, () -> EntityInfo>()
    private val cameras = mutableMapOf<String, () -> Camera>()
    private val lights = mutableMapOf<String, () -> Light>()
    val sounds = mutableMapOf<String, () -> Sound>()

    private var onSceneCreated: ((scene: Scene) -> Unit)? = null
    private var onSceneUpdate: OnSceneUpdate? = null
    private var onSceneBeforeUpdate: OnSceneBeforeUpdate? = null
    private var onSceneAfterUpdate: OnSceneAfterUpdate? = null

    fun camera(id: String, builder: CameraBuilder.() -> Unit) {
        if (cameras.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        cameras[id] = {
            CameraBuilder().apply(builder).build()
        }
    }

    fun light(id: String, builder: LightBuilder.() -> Unit) {
        if (lights.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        lights[id] = {
            LightBuilder().apply(builder).build()
        }
    }

    @OptIn(ExperimentalForeignApi::class)
    fun entity(id: String, modelPath: String, builder: EntityBuilder.() -> Unit) {
        if (entities.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        entities[id] = {
            EntityBuilder(modelPath).apply(builder).build()
        }
    }

    fun sound(id: String, soundPath: String, builder: SoundBuilder.() -> Unit = {}) {
        if (sounds.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        sounds[id] = {
            SoundBuilder(soundPath).apply(builder).build()
        }
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): SceneInfo {
        val scene = Scene()
        cameras.forEach { (id, builder) ->
            scene.cameras[id] = builder.invoke()
        }
        lights.forEach { (id, builder) ->
            scene.lights[id] = builder.invoke()
        }
        entities.forEach { (id, builder) ->
            scene.entities[id] = builder.invoke()
        }
        sounds.forEach { (id, builder) ->
            scene.sounds[id] = builder.invoke()
        }

        return SceneInfo(
            scene,
            onSceneCreated,
            onSceneUpdate,
            onSceneBeforeUpdate,
            onSceneAfterUpdate
        )
    }

    fun onSceneCreated(block: (Scene) -> Unit) {
        onSceneCreated = block
    }

    fun onSceneUpdate(block: OnSceneUpdate) {
        onSceneUpdate = block
    }

    fun onBeforeSceneUpdate(block: OnSceneBeforeUpdate) {
        onSceneBeforeUpdate = block
    }

    fun onAfterSceneUpdate(block: OnSceneAfterUpdate) {
        onSceneAfterUpdate = block
    }
}

fun Application.setActiveScene(id: String) {
    val builder = sceneBuilders[id] ?: throw Exception("Scene with id $id does not exist")
    val existingScene = scenes[id]
    if (existingScene != null) {
        activeScene = existingScene
    } else {
        // Create new scene
        val sceneInfo = builder.invoke()

        // Only once if the scene is new
        sceneInfo.onSceneCreated?.invoke(sceneInfo.scene)

        // Initialize behaviours
        sceneInfo.scene.entities.forEach {
            it.value.behaviors.forEach { behavior ->
                behavior.initialize(sceneInfo.scene, it.value.entity)
            }
            it.value.onSceneEntityUpdate
        }

        // Set as active scene
        scenes[id] = sceneInfo
        activeScene = sceneInfo
    }
}

