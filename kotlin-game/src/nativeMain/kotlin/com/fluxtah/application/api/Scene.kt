package com.fluxtah.application.api

import kotlinx.cinterop.ExperimentalForeignApi

private val sceneBuilders = mutableMapOf<String, () -> Scene>()
private val scenes = mutableMapOf<String, Scene>()

@DslMarker
annotation class SceneDsl

@OptIn(ExperimentalForeignApi::class)
class Scene {
    val cameras = mutableMapOf<String, Camera>()
    val lights = mutableMapOf<String, Light>()
    val entities = mutableMapOf<String, Entity>()
}

fun Application.scene(id: String, builder: SceneBuilder.() -> Unit) {
    sceneBuilders[id] = {
        SceneBuilder(id).apply(builder).build()
    }
}

@SceneDsl
class SceneBuilder(val sceneId: String) {
    private val entities = mutableMapOf<String, () -> Entity>()
    private val cameras = mutableMapOf<String, () -> Camera>()
    private val lights = mutableMapOf<String, () -> Light>()
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
    fun build(): Scene {
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
        return scene
    }
}

fun Application.setActiveScene(id: String) {
    val builder = sceneBuilders[id] ?: throw Exception("Scene with id $id does not exist")
    if (scenes.containsKey(id)) return // Already active

    val scene = builder.invoke()

    // Set as active scene
    scenes[id] = scene
}

