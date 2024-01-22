package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.entity.EntityBehavior
import com.fluxtah.application.api.fixedTimeStep

class YawBehavior(
    private val yawLeft: () -> Boolean,
    private val yawRight: () -> Boolean
) : EntityBehavior() {
    private val yawSpeed = 50.0f // Increased for faster rotation

    override fun update(time: Float) {
        var yawDirection = 0.0f
        if (yawLeft()) {
            yawDirection -= 1.0f
        }
        if (yawRight()) {
            yawDirection += 1.0f
        }

        val yawIncrement = yawSpeed * yawDirection * fixedTimeStep

        // Apply the yaw increment to the entity's rotation
        entity.rotate(0.0f, -yawIncrement, 0.0f)
    }
}
