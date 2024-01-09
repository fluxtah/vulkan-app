package com.fluxtah.application.api

import com.fluxtah.application.api.interop.c_setActiveCamera
import kotlinx.cinterop.ExperimentalForeignApi

var activeScene: SceneInfo = SceneInfo(Scene.EMPTY())
private val sceneBuilders = mutableMapOf<String, () -> SceneInfo>()
private val scenes = mutableMapOf<String, SceneInfo>()

data class SceneInfo(
    val scene: Scene,
    val onSceneCreated: ((Scene) -> Unit)? = null,
    val onSceneUpdate: OnSceneUpdate? = null,
    val onSceneBeforeUpdate: OnSceneBeforeUpdate? = null,
    val onSceneAfterUpdate: OnSceneAfterUpdate? = null
)

data class EntityInfo(
    val entity: Entity,
    val onSceneEntityUpdate: OnSceneEntityUpdate?,
    val onSceneBeforeEntityUpdate: OnSceneBeforeEntityUpdate?,
    val onSceneAfterEntityUpdate: OnSceneAfterEntityUpdate?,
    val behaviors: MutableList<EntityBehavior>
)

data class EntityPoolInfo(
    val factory: () -> Entity,
    val entities: MutableList<Entity>,
    val size: Int,
    val behaviors: MutableList<EntityBehavior>
)


@DslMarker
annotation class SceneDsl

interface Scene {
    fun setActiveCamera(id: String)
    fun activeCamera(): Camera?
    fun cameraById(id: String): Camera?

    fun entityById(id: String): Entity?

    fun soundById(id: String): Sound?

    class EMPTY : Scene {
        override fun setActiveCamera(id: String) = Unit
        override fun activeCamera(): Camera? = null
        override fun cameraById(id: String): Camera? = null

        override fun entityById(id: String): Entity? = null
        override fun soundById(id: String): Sound? = null
    }
}

@OptIn(ExperimentalForeignApi::class)
class SceneImpl : Scene {
    private var activeCamera: Camera? = null
    internal val cameras = mutableMapOf<String, Camera>()
    internal val lights = mutableMapOf<String, Light>()
    internal val entities = mutableMapOf<String, EntityInfo>()
    internal val sounds = mutableMapOf<String, Sound>()

    override fun setActiveCamera(id: String) {
        activeCamera = cameras[id] ?: throw Exception("Camera with id $id does not exist")
        c_setActiveCamera!!.invoke(ApplicationContext.vulcanContext!!, activeCamera!!.handle)
    }

    override fun activeCamera(): Camera? {
        return activeCamera
    }

    override fun cameraById(id: String): Camera? {
        return cameras[id]
    }

    override fun entityById(id: String): Entity? {
        return entities[id]?.entity
    }

    override fun soundById(id: String): Sound? {
        return sounds[id]
    }
}

typealias OnSceneUpdate = ((scene: Scene, time: Float) -> Unit)
typealias OnSceneBeforeUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneEntityUpdate = ((scene: Scene, entity: Entity, time: Float) -> Unit)
typealias OnSceneBeforeEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)

//fun Application.scene(): Scene {
//    return activeScene.scene
//}

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
    private val entityPools = mutableMapOf<String, () -> EntityInfo>()
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

//    @OptIn(ExperimentalForeignApi::class)
//    fun entityPool(id: String, modelPath: String, initialSize: Int, builder: EntityPoolBuilder.() -> Unit) {
//        if (entityPools.containsKey(id)) {
//            throw Exception("Entity pool with id $id already exists")
//        }
//        entityPools[id] = {
//            EntityPoolBuilder(modelPath).apply(builder).build()
//        }
//    }

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
        (sceneInfo.scene as SceneImpl).entities.forEach {
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

