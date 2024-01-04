package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.SceneBuilder
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.isKeyPressed

fun SceneBuilder.spaceship() = entity("ship", "models/ship.glb") {
    var velocityY = 0.0f
    var positionY = 0.0f
    val gravity = 2.0f
    val shipMass = 200.0f
    var thrust = 0.0f
    val maxThrust = 30000.0f
    val maxAltitude = 3.0f
    val thrustIncrement = 9000f
    val overshootDamping = 0.95f
    val hoverDamping = 0.99f
    val maxVelocity = 3.0f

    val fixedTimeStep = 1.0f / 60.0f // Fixed timestep (e.g., 60 updates per second)
    var accumulatedTime = 0.0f

    position(0.0f, 0.0f, 0.0f)

    onSceneUpdate { _, entity, _, deltaTime ->
        accumulatedTime += deltaTime

        // Physics update happens only at each fixed timestep
        while (accumulatedTime >= fixedTimeStep) {
            // Adjust thrust based on key input
            if (isKeyPressed(Key.Equal)) {
                thrust = (thrust + thrustIncrement * fixedTimeStep).coerceAtMost(maxThrust)
            } else if (thrust > 0.0f) {
                thrust = (thrust - thrustIncrement * fixedTimeStep).coerceAtLeast(0.0f)
            }

            // Calculate forces and acceleration
            val gravitationalForce = shipMass * gravity
            val netForce = thrust - gravitationalForce
            val acceleration = netForce / shipMass

            // Update velocity and position
            velocityY += acceleration * fixedTimeStep
            positionY += velocityY * fixedTimeStep

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

            accumulatedTime -= fixedTimeStep
        }

        entity.position(y = positionY)
    }
}
