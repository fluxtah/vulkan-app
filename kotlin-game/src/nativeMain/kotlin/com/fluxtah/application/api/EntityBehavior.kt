package com.fluxtah.application.api

interface EntityBehavior {
    fun initialize(scene: Scene, entity: Entity) {}
    fun beforeUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {}
    fun update(scene: Scene, entity: Entity, time: Float)
    fun afterUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {}
}