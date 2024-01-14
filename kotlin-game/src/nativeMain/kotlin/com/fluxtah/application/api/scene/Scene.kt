package com.fluxtah.application.api.scene

import com.fluxtah.application.api.*
import com.fluxtah.application.api.entity.Entity
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.StableRef

var activeSceneInfo: SceneInfo = SceneInfo(Scene.EMPTY())
private val sceneBuilders = mutableMapOf<String, () -> SceneInfo>()
private val scenes = mutableMapOf<String, SceneInfo>()

data class SceneInfo(
    val scene: Scene,
    val onSceneCreated: ((Scene) -> Unit)? = null,
    val onSceneUpdate: OnSceneUpdate? = null,
    val onSceneBeforeUpdate: OnSceneBeforeUpdate? = null,
    val onSceneAfterUpdate: OnSceneAfterUpdate? = null
)

@OptIn(ExperimentalForeignApi::class)
data class EntityInfo(
    val entity: Entity,
    val onSceneEntityUpdate: OnSceneEntityUpdate? = null,
    val onSceneBeforeEntityUpdate: OnSceneBeforeEntityUpdate? = null,
    val onSceneAfterEntityUpdate: OnSceneAfterEntityUpdate? = null,
    val behaviors: List<EntityBehavior>,
    val onCollision: OnCollision? = null,
    var stableRef: StableRef<EntityInfo>? = null
)

data class EntityPoolInfo(
    val initialSize: Int,
    val factory: () -> EntityInfo,
    val entitiesAvailable: MutableList<EntityInfo>,
    val entitiesInUse: MutableList<EntityInfo>,
    val onCollision: OnCollision? = null
)

@DslMarker
annotation class SceneDsl

interface Scene {
    fun setActiveCamera(id: String)
    fun activeCamera(): Camera?
    fun cameraById(id: String): Camera?

    fun entityById(id: String): Entity?
    fun entityFromPool(id: String, block: (entity: Entity) -> Unit)
    fun entityToPool(entity: Entity)

    fun soundById(id: String): Sound?

    class EMPTY : Scene {
        override fun setActiveCamera(id: String) = Unit
        override fun activeCamera(): Camera? = null
        override fun cameraById(id: String): Camera? = null

        override fun entityById(id: String): Entity? = null
        override fun entityFromPool(id: String, block: (entity: Entity) -> Unit) = Unit
        override fun entityToPool(entity: Entity) = Unit

        override fun soundById(id: String): Sound? = null
    }
}

typealias OnSceneUpdate = ((scene: Scene, time: Float) -> Unit)
typealias OnSceneBeforeUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterUpdate = ((scene: Scene, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneEntityUpdate = ((scene: Scene, entity: Entity, time: Float) -> Unit)
typealias OnSceneBeforeEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)
typealias OnSceneAfterEntityUpdate = ((scene: Scene, entity: Entity, time: Float, deltaTime: Float) -> Unit)
typealias OnCollision = ((scene: Scene, entity: Entity, entities: List<Entity>) -> Unit)

fun Application.scene(id: String, block: SceneBuilder.() -> Unit) {
    sceneBuilders[id] = {
        val builder = SceneBuilder(id)
        block(builder)
        builder.build()
    }
}

fun Application.setActiveScene(id: String) {
    val builder = sceneBuilders[id] ?: throw Exception("Scene with id $id does not exist")
    val existingScene = scenes[id]
    if (existingScene != null) {
        activeSceneInfo = existingScene
    } else {
        // Create new scene
        val sceneInfo = builder.invoke()

        // Only once if the scene is new
        sceneInfo.onSceneCreated?.invoke(sceneInfo.scene)

        // Initialize behaviours
        (sceneInfo.scene as SceneImpl).entities.forEach {
            it.value.behaviors.forEach { behavior ->
                behavior.initialize()
            }
            it.value.onSceneEntityUpdate
        }

        // Initialize entity pools with active entities
        sceneInfo.scene.entityPools.forEach {
            it.value.entitiesInUse.forEach { entityInfo ->
                entityInfo.behaviors.forEach { behavior ->
                    behavior.initialize()
                }
            }
        }

        // Set as active scene
        scenes[id] = sceneInfo
        activeSceneInfo = sceneInfo
    }
}

