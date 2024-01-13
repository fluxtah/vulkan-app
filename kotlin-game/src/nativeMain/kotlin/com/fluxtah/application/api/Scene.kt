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
    val onSceneEntityUpdate: OnSceneEntityUpdate? = null,
    val onSceneBeforeEntityUpdate: OnSceneBeforeEntityUpdate? = null,
    val onSceneAfterEntityUpdate: OnSceneAfterEntityUpdate? = null,
    val behaviors: List<EntityBehavior>
)

data class EntityPoolInfo(
    val initialSize: Int,
    val factory: () -> EntityInfo,
    val entitiesAvailable: MutableList<EntityInfo>,
    val entitiesInUse: MutableList<EntityInfo>
)

@DslMarker
annotation class SceneDsl

interface Scene {
    fun setActiveCamera(id: String)
    fun activeCamera(): Camera?
    fun cameraById(id: String): Camera?

    fun entityById(id: String): Entity?
    fun entityFromPool(id: String, block: (entity: Entity, behaviors: List<EntityBehavior>) -> Unit)
    fun entityToPool(entity: Entity)

    fun soundById(id: String): Sound?

    class EMPTY : Scene {
        override fun setActiveCamera(id: String) = Unit
        override fun activeCamera(): Camera? = null
        override fun cameraById(id: String): Camera? = null

        override fun entityById(id: String): Entity? = null
        override fun entityFromPool(id: String, block: (entity: Entity, behaviors: List<EntityBehavior>) -> Unit) = Unit
        override fun entityToPool(bolt: Entity) = Unit

        override fun soundById(id: String): Sound? = null
    }
}

@OptIn(ExperimentalForeignApi::class)
class SceneImpl : Scene {
    private var activeCamera: Camera? = null
    internal val cameras = mutableMapOf<String, Camera>()
    internal val lights = mutableMapOf<String, Light>()
    internal val entities = mutableMapOf<String, EntityInfo>()
    internal val entityPools = mutableMapOf<String, EntityPoolInfo>()
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

    override fun entityFromPool(id: String, block: (entity: Entity, behaviors: List<EntityBehavior>) -> Unit) {
        val pool = entityPools[id] ?: throw Exception("Entity pool with id $id does not exist")

        if (pool.entitiesAvailable.any()) {
            val entity = pool.entitiesAvailable.removeAt(0)
            block(entity.entity, entity.behaviors)
            entity.behaviors.forEach { behavior -> behavior.initialize(this, entity.entity) }
            pool.entitiesInUse.add(entity)
        }
        // TODO: Grow dynamically if specified
//        else {
//            val entity = pool.factory.invoke()
//            block(entity.entity)
//            pool.entitiesInUse.add(entity)
//        }
    }

    override fun entityToPool(entity: Entity) {
        val pool = entityPools[entity.id] ?: throw Exception("Entity with ${entity.id} is not from a pool")
        val entityInfo =
            pool.entitiesInUse.find { it.entity == entity } ?: throw Exception("Entity with ${entity.id} is not in use")
        pool.entitiesInUse.remove(entityInfo)
        pool.entitiesAvailable.add(entityInfo)
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
    private val entityPools = mutableMapOf<String, () -> EntityPoolInfo>()
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
            EntityBuilder(id, modelPath).apply(builder).build()
        }
    }

    fun entityPool(id: String, modelPath: String, builder: EntityPoolBuilder.() -> Unit) {
        if (entityPools.containsKey(id)) {
            throw Exception("Entity pool with id $id already exists")
        }
        entityPools[id] = {
            EntityPoolBuilder(id, modelPath).apply(builder).build()
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
        entityPools.forEach { (id, builder) ->
            scene.entityPools[id] = builder.invoke()
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

        // Initialize entity pools with active entities
        sceneInfo.scene.entityPools.forEach {
            it.value.entitiesInUse.forEach { entityInfo ->
                entityInfo.behaviors.forEach { behavior ->
                    behavior.initialize(sceneInfo.scene, entityInfo.entity)
                }
            }
        }

        // Set as active scene
        scenes[id] = sceneInfo
        activeScene = sceneInfo
    }
}

