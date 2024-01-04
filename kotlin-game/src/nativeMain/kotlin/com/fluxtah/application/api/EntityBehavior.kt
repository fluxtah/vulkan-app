package com.fluxtah.application.api

interface EntityBehavior {
    fun update(scene: Scene, entity: Entity, time: Float, deltaTime: Float)
}