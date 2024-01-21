package com.fluxtah.application.api.scene

import com.fluxtah.application.api.*
import com.fluxtah.application.api.emitter.EmitterBuilder
import com.fluxtah.application.api.emitter.EmitterPoolBuilder
import com.fluxtah.application.api.entity.EntityBuilder
import com.fluxtah.application.api.entity.EntityPoolBuilder
import kotlinx.cinterop.ExperimentalForeignApi

@SceneDsl
class SceneBuilder(val sceneId: String) {
    private val lights = mutableMapOf<String, (Scene) -> Light>()
    private val cameras = mutableMapOf<String, (Scene) -> Camera>()

    private val entities = mutableMapOf<String, (Scene) -> EntityInfo>()
    private val entityPools = mutableMapOf<String, (Scene) -> EntityPoolInfo>()

    private val emitters = mutableMapOf<String, (Scene) -> EmitterInfo>()
    private val emitterPools = mutableMapOf<String, (Scene) -> EmitterPoolInfo>()

    private val sounds = mutableMapOf<String, (Scene) -> Sound>()

    private var onSceneCreated: ((scene: Scene) -> Unit)? = null
    private var onSceneUpdate: OnSceneUpdate? = null
    private var onSceneBeforeUpdate: OnSceneBeforeUpdate? = null
    private var onSceneAfterUpdate: OnSceneAfterUpdate? = null

    fun light(id: String, builder: LightBuilder.() -> Unit) {
        if (lights.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        lights[id] = {
            LightBuilder().apply(builder).build()
        }
    }

    fun camera(id: String, builder: CameraBuilder.() -> Unit) {
        if (cameras.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        cameras[id] = {
            CameraBuilder().apply(builder).build()
        }
    }

    @OptIn(ExperimentalForeignApi::class)
    fun entity(id: String, modelPath: String, builder: EntityBuilder.() -> Unit) {
        if (entities.containsKey(id)) {
            throw Exception("Entity with id $id already exists")
        }
        entities[id] = {
            EntityBuilder(it, id, modelPath).apply(builder).build()
        }
    }

    fun entityPool(id: String, modelPath: String, builder: EntityPoolBuilder.() -> Unit) {
        if (entityPools.containsKey(id)) {
            throw Exception("Entity pool with id $id already exists")
        }
        entityPools[id] = {
            EntityPoolBuilder(it, id, modelPath).apply(builder).build()
        }
    }

    fun emitter(id: String, modelPath: String, builder: EmitterBuilder.() -> Unit) {
        if (emitters.containsKey(id)) {
            throw Exception("Emitter with id $id already exists")
        }
        emitters[id] = {
            EmitterBuilder(it, id, modelPath).apply(builder).build()
        }
    }

    fun emitterPool(id: String, modelPath: String, builder: EmitterPoolBuilder.() -> Unit) {
        if (emitterPools.containsKey(id)) {
            throw Exception("Emitter pool with id $id already exists")
        }
        emitterPools[id] = {
            EmitterPoolBuilder(it, id, modelPath).apply(builder).build()
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
        val scene = SceneImpl()
        lights.forEach { (id, builder) ->
            scene.lights[id] = builder.invoke(scene)
        }
        cameras.forEach { (id, builder) ->
            scene.cameras[id] = builder.invoke(scene)
        }
        entities.forEach { (id, builder) ->
            scene.entities[id] = builder.invoke(scene)
        }
        entityPools.forEach { (id, builder) ->
            scene.entityPools[id] = builder.invoke(scene)
        }
        emitters.forEach { (id, builder) ->
            scene.emitters[id] = builder.invoke(scene)
        }
        emitterPools.forEach { (id, builder) ->
            scene.emitterPools[id] = builder.invoke(scene)
        }

        sounds.forEach { (id, builder) ->
            scene.sounds[id] = builder.invoke(scene)
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