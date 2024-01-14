package com.fluxtah.application.api.scene

import com.fluxtah.application.api.ApplicationContext
import com.fluxtah.application.api.Camera
import com.fluxtah.application.api.Light
import com.fluxtah.application.api.Sound
import com.fluxtah.application.api.entity.Entity
import com.fluxtah.application.api.interop.c_setActiveCamera
import kotlinx.cinterop.ExperimentalForeignApi

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

    override fun entityFromPool(id: String, block: (entity: Entity) -> Unit) {
        val pool = entityPools[id] ?: throw Exception("Entity pool with id $id does not exist")

        if (pool.entitiesAvailable.any()) {
            val entity = pool.entitiesAvailable.removeAt(0)
            block(entity.entity)
            entity.behaviors.forEach { behavior -> behavior.initialize() }
            pool.entitiesInUse.add(entity)
            entity.entity.inUse = true
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
            pool.entitiesInUse.find { it.entity == entity }
                ?: throw Exception("Entity with ${entity.id} is not in use, check inUse before adding it back to the pool")
        pool.entitiesInUse.remove(entityInfo)
        entity.inUse = false
        pool.entitiesAvailable.add(entityInfo)
    }

    override fun soundById(id: String): Sound? {
        return sounds[id]
    }
}