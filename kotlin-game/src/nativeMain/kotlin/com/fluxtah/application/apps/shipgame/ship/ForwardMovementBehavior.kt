package com.fluxtah.application.apps.shipgame.ship

import com.fluxtah.application.api.Entity
import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.Scene
import com.fluxtah.application.apps.shipgame.Vector3
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.sin

class ForwardMovementBehavior(private val isMovingForward: () -> Boolean) : EntityBehavior {
    private var forwardVelocity = 0.0f
    private val forwardAcceleration = 1.0f // Adjust for desired acceleration
    private val maxForwardSpeed = 5.0f // Adjust for max speed

    override fun update(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {

        //    while (accumulatedTime >= fixedTimeStep) {
        forwardVelocity = if (isMovingForward()) {
            // Increase forward velocity
            (forwardVelocity + forwardAcceleration * deltaTime).coerceAtMost(maxForwardSpeed)
        } else {
            // Slow down if not moving forward
            (forwardVelocity - forwardAcceleration * deltaTime).coerceAtLeast(0.0f)
        }

        // Calculate new position based on forward velocity
        val newPosition = Vector3(entity.positionX, entity.positionY, entity.positionZ) + calculateForwardMovement(
            entity.rotationY,
            forwardVelocity * deltaTime
        )

        entity.position(newPosition.x, newPosition.y, newPosition.z)
        //   }
    }

    private fun calculateForwardMovement(yaw: Float, distance: Float): Vector3 {
        // Convert yaw to radians
        val yawRadians = yaw * (PI / 180.0).toFloat()
        val sinYaw = sin(yawRadians)
        val cosYaw = cos(yawRadians)

        // Calculate forward movement based on yaw
        return Vector3(
            distance * sinYaw,
            0f, // Assuming no vertical movement in forward thrust
            distance * cosYaw
        )
    }
}
