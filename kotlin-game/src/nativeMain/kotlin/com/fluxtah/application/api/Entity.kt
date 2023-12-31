package com.fluxtah.application.api

import com.fluxtah.application.api.interop.CEntity
import com.fluxtah.application.api.interop.c_createEntity
import com.fluxtah.application.api.interop.c_destroyEntity
import kotlinx.cinterop.ExperimentalForeignApi

@OptIn(ExperimentalForeignApi::class)
class Entity(private val handle: CEntity) {
    fun destroy() {
        c_destroyEntity!!.invoke(ApplicationContext.vulcanContext!!, handle)
    }
}

@SceneDsl
class EntityBuilder(private val modelPath: String) {
    fun position(x: Float, y: Float, z: Float) { /* ... */
    }

    fun rotate(x: Float = 0f, y: Float = 0f, z: Float = 0f) { /* ... */
    }

    fun onEachFrame(action: (deltaTime: Float) -> Unit) { /* ... */
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): Entity {
        val entity = c_createEntity!!.invoke(ApplicationContext.vulcanContext!!, modelPath)
        return Entity(entity)
    }
}
