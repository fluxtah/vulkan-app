package com.fluxtah.application.api

class Entity

@SceneDsl
class EntityBuilder(val modelPath: String) {
    fun position(x: Float, y: Float, z: Float) { /* ... */
    }

    fun rotate(x: Float = 0f, y: Float = 0f, z: Float = 0f) { /* ... */
    }

    fun onEachFrame(action: (deltaTime: Float) -> Unit) { /* ... */
    }
    // Additional methods for transformations and behaviors
    fun build(): Entity {
        return Entity()
    }
}
