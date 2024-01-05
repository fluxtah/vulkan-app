package com.fluxtah.application.apps.shipgame.ship

import com.fluxtah.application.api.Entity
import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.Scene

class YawBehavior(
    private val yawLeft: () -> Boolean,
    private val yawRight: () -> Boolean
) : EntityBehavior {
    private val yawSpeed = 50.0f // Increased for faster rotation

    override fun update(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
        var yawDirection = 0.0f
        if (yawLeft()) {
            yawDirection -= 1.0f
        }
        if (yawRight()) {
            yawDirection += 1.0f
        }

        val yawIncrement = yawSpeed * yawDirection * deltaTime

        // Apply the yaw increment to the entity's rotation
        entity.rotate(0.0f, -yawIncrement, 0.0f)
    }
}
