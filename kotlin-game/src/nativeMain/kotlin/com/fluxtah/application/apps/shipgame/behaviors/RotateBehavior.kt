package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.Entity
import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.Scene
import com.fluxtah.application.api.fixedTimeStep


class RotateBehavior(
    private val speedX: Float = 0.0f,
    private val speedY: Float = 0.0f,
    private val speedZ: Float = 0.0f
) : EntityBehavior {
    override fun update(scene: Scene, entity: Entity, time: Float) {
        entity.setRotation(
            entity.rotationX + (fixedTimeStep * speedX),
            entity.rotationY + (fixedTimeStep * speedY),
            entity.rotationZ + (fixedTimeStep * speedZ)
        )
    }
}