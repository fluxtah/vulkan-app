package com.fluxtah.application.apps.shipgame.ship

import com.fluxtah.application.api.Entity
import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.Scene
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.isKeyPressed

class ThrustBehavior(private val isThrusting: () -> Boolean) : EntityBehavior {
    private var velocityY = 0.0f
    private var positionY = 0.0f
    private val gravity = 2.0f
    private val shipMass = 200.0f
    private var thrust = 0.0f
    private val maxThrust = 30000.0f
    private val maxAltitude = 3.0f
    private val thrustIncrement = 9000f
    private val overshootDamping = 0.95f
    private val hoverDamping = 0.99f
    private val maxVelocity = 3.0f

    override fun update(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
        // Adjust thrust based on key input
        if (isThrusting()) {
            thrust = (thrust + thrustIncrement * deltaTime).coerceAtMost(maxThrust)
        } else if (thrust > 0.0f) {
            thrust = (thrust - thrustIncrement * deltaTime).coerceAtLeast(0.0f)
        }

        // Calculate forces and acceleration
        val gravitationalForce = shipMass * gravity
        val netForce = thrust - gravitationalForce
        val acceleration = netForce / shipMass

        // Update velocity and position
        velocityY += acceleration * deltaTime
        positionY += velocityY * deltaTime

        // Apply max velocity
        velocityY = velocityY.coerceAtMost(maxVelocity)

        // Apply damping if overshooting max altitude
        if (positionY > maxAltitude) {
            velocityY *= overshootDamping
            positionY = maxAltitude + (positionY - maxAltitude) * overshootDamping
        }

        // Apply hover damping for smooth hovering
        if (positionY >= maxAltitude && thrust > gravitationalForce) {
            thrust = gravitationalForce
            velocityY *= hoverDamping
        }

        // Ensure the spaceship doesn't go below the ground
        positionY = positionY.coerceAtLeast(0.0f)

        entity.position(y = positionY)
    }
}