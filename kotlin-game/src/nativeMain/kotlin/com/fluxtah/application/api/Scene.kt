package com.fluxtah.application.api

import com.fluxtah.application.api.interop.c_setActiveCamera
import kotlinx.cinterop.ExperimentalForeignApi

var activeScene: SceneInfo? = null
private val sceneBuilders = mutableMapOf<String, () -> SceneInfo>()
private val scenes = mutableMapOf<String, SceneInfo>()

data class SceneInfo(
    val scene: Scene,
    val onSceneCreated: ((Scene) -> Unit)?
)

@DslMarker
annotation class SceneDsl

@OptIn(ExperimentalForeignApi::class)
class Scene {
    private var activeCamera: Camera? = null
    val cameras = mutableMapOf<String, Camera>()
    val lights = mutableMapOf<String, Light>()
    val entities = mutableMapOf<String, Entity>()

    fun setActiveCamera(id: String) {
        activeCamera = cameras[id] ?: throw Exception("Camera with id $id does not exist")
        c_setActiveCamera!!.invoke(ApplicationContext.vulcanContext!!, activeCamera!!.handle)
    }

    fun activeCamera(): Camera? {
        return activeCamera
    }
}

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
    private val entities = mutableMapOf<String, () -> Entity>()
    private val cameras = mutableMapOf<String, () -> Camera>()
    private val lights = mutableMapOf<String, () -> Light>()

    private var onSceneCreated: ((Scene) -> Unit)? = null

    fun camera(id: String, builder: CameraBuilder.() -> Unit) {
        cameras[id] = {
            CameraBuilder().apply(builder).build()
        }
    }

    fun light(id: String, builder: LightBuilder.() -> Unit) {
        lights[id] = {
            LightBuilder().apply(builder).build()
        }
    }

    @OptIn(ExperimentalForeignApi::class)
    fun entity(id: String, modelPath: String, builder: EntityBuilder.() -> Unit) {
        entities[id] = {
            EntityBuilder(modelPath).apply(builder).build()
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
        return SceneInfo(
            scene,
            onSceneCreated
        )
    }

    fun onSceneCreated(block: (Scene) -> Unit) {
        onSceneCreated = block
    }
}

fun Application.setActiveScene(id: String) {
    val builder = sceneBuilders[id] ?: throw Exception("Scene with id $id does not exist")
    val existingScene = scenes[id]
    if (existingScene != null) {
        activeScene = existingScene
    } else {
        // Create new scene
        val scene = builder.invoke()

        scene.onSceneCreated?.invoke(scene.scene)

        // Set as active scene
        scenes[id] = scene
        activeScene = scene
    }
}

