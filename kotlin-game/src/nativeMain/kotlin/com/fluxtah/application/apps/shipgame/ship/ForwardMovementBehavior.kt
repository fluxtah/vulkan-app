package com.fluxtah.application.apps.shipgame.ship

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.sin

class ForwardMovementBehavior(
    private val isMovingForward: () -> Boolean,
    private val isBraking: () -> Boolean,
    ) : EntityBehavior {
    private var forwardVelocity = 0.0f
    private val forwardAcceleration = 1.0f // Adjust for desired acceleration
    private val maxForwardSpeed = 5.0f // Adjust for max speed

    private lateinit var engineSound: Sound

    override fun initialize(scene: Scene, entity: Entity) {
        engineSound = scene.sounds["engine"]!!
        engineSound.playIfNotPlaying()
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {

        forwardVelocity = if (isMovingForward()) {
            // Increase forward velocity
            (forwardVelocity + forwardAcceleration * fixedTimeStep).coerceAtMost(maxForwardSpeed)
        } else {
            // Slow down if not moving forward
            (forwardVelocity - forwardAcceleration * fixedTimeStep).coerceAtLeast(0.0f)
        }

        if(isBraking()) {
            forwardVelocity = lerp(forwardVelocity, 0.0f, 0.05f)
        }

        // Calculate new position based on forward velocity
        val newPosition = Vector3(entity.positionX, entity.positionY, entity.positionZ) + calculateForwardMovement(
            entity.rotationY,
            forwardVelocity * fixedTimeStep
        )

        entity.position(newPosition.x, newPosition.y, newPosition.z)
    }

    fun lerp(a: Float, b: Float, f: Float): Float {
        return a + f * (b - a)
    }

    override fun afterUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
        engineSound.setPitch(0.7f + (forwardVelocity / maxForwardSpeed) * 0.6f)
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
