package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.Sound
import com.fluxtah.application.api.fixedTimeStep
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.toRadians
import com.fluxtah.application.apps.shipgame.Id
import kotlin.math.cos
import kotlin.math.sin

class LateralMovementBehavior(
    private val isMovingLeft: () -> Boolean,
    private val isMovingRight: () -> Boolean,
    private val lateralAcceleration: Float = 1.0f,
    private val maxLateralSpeed: Float = 5.0f,
) : EntityBehavior() {
    private var lateralVelocity = 0.0f
    private lateinit var movementSound: Sound

    override fun initialize() {
        movementSound = scene.soundById(Id.SOUND_LATERAL_THRUST)!!
    }

    override fun update(time: Float) {
        lateralVelocity = when {
            isMovingRight() -> {
                movementSound.playIfNotPlaying()
                // Increase lateral velocity to the right
                (lateralVelocity + lateralAcceleration * fixedTimeStep).coerceAtMost(maxLateralSpeed)
            }

            isMovingLeft() -> {
                movementSound.playIfNotPlaying()
                // Increase lateral velocity to the left (negative value)
                (lateralVelocity - lateralAcceleration * fixedTimeStep).coerceAtLeast(-maxLateralSpeed)
            }

            else -> {
                movementSound.stopIfPlaying()
                // Slow down to a halt if not moving left or right
                if (lateralVelocity > 0) {
                    (lateralVelocity - lateralAcceleration * fixedTimeStep).coerceAtLeast(0.0f)
                } else {
                    (lateralVelocity + lateralAcceleration * fixedTimeStep).coerceAtMost(0.0f)
                }
            }
        }

        // Calculate new position based on lateral velocity and spaceship's orientation
        val newPosition = Vector3(entity.positionX, entity.positionY, entity.positionZ) + calculateLateralMovement(
            entity.rotationY,
            lateralVelocity * fixedTimeStep
        )

        entity.setPosition(newPosition.x, newPosition.y, newPosition.z)
    }

    private fun calculateLateralMovement(yaw: Float, distance: Float): Vector3 {
        // Convert yaw to radians
        val yawRadians = yaw.toRadians()

        // Calculate forward direction based on yaw
        val forwardDirection = Vector3(
            sin(yawRadians),
            0f,
            cos(yawRadians)
        )

        // Calculate right direction by rotating forward direction by -90 degrees around the up axis
        val leftDirection = Vector3(
            cos(yawRadians),
            0f,
            -sin(yawRadians)
        )

        // Calculate left direction by rotating forward direction by 90 degrees around the up axis
        val rightDirection = Vector3(
            -cos(yawRadians),
            0f,
            sin(yawRadians)
        )

        // Determine the direction of lateral movement (right or left)
        val lateralDirection = if (lateralVelocity > 0) rightDirection else -leftDirection

        // Return the lateral movement vector, scaled by the distance
        return lateralDirection * distance
    }

}
