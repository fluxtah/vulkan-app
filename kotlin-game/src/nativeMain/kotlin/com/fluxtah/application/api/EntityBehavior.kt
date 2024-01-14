package com.fluxtah.application.api

import com.fluxtah.application.api.entity.Entity
import com.fluxtah.application.api.scene.Scene

abstract class EntityBehavior {
    lateinit var entity: Entity
    lateinit var scene: Scene
    open fun initialize() {}
    open fun beforeUpdate(time: Float, deltaTime: Float) {}
    open fun update(time: Float) {}
    open fun afterUpdate(time: Float, deltaTime: Float) {}
}