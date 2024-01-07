package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.toRadians
import kotlin.math.PI
import kotlin.math.abs
import kotlin.math.cos
import kotlin.math.sin

class ForwardMovementBehavior(
    private val isMovingForward: () -> Boolean,
    private val isReversing: () -> Boolean,
) : EntityBehavior {
    private var forwardVelocity = 0.0f
    private val acceleration = 1.0f // Adjust for desired acceleration
    private val reversingFactor = 2.0f // Adjust for desired reverse speed
    private val maxForwardSpeed = 5.0f // Adjust for max speed
    private val maxReverseSpeed = -1.0f // Adjust for max reverse speed

    private lateinit var engineSound: Sound

    override fun initialize(scene: Scene, entity: Entity) {
        engineSound = scene.sounds["engine"]!!
        engineSound.playIfNotPlaying()
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {

        forwardVelocity = when {
            isMovingForward() -> {
                // Increase forward velocity
                (forwardVelocity + acceleration * fixedTimeStep).coerceAtMost(maxForwardSpeed)
            }
            isReversing() -> {
                // Decrease forward velocity for reverse movement
                (forwardVelocity - (acceleration * reversingFactor) * fixedTimeStep).coerceAtLeast(maxReverseSpeed)
            }
            else -> {
                // Slow down to a halt if not moving forward or reversing
                if (forwardVelocity > 0) {
                    (forwardVelocity - acceleration * fixedTimeStep).coerceAtLeast(0.0f)
                } else {
                    (forwardVelocity + acceleration * fixedTimeStep).coerceAtMost(0.0f)
                }
            }
        }

        // Calculate new position based on forward velocity
        val newPosition = Vector3(entity.positionX, entity.positionY, entity.positionZ) + calculateForwardMovement(
            entity.rotationY,
            forwardVelocity * fixedTimeStep
        )

        entity.setPosition(newPosition.x, newPosition.y, newPosition.z)
    }

    fun lerp(a: Float, b: Float, f: Float): Float {
        return a + f * (b - a)
    }

    override fun afterUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
        engineSound.setPitch(abs(0.7f + (forwardVelocity / maxForwardSpeed) * 0.6f))
    }

    private fun calculateForwardMovement(yaw: Float, distance: Float): Vector3 {
        // Convert yaw to radians
        val yawRadians = yaw.toRadians()
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
