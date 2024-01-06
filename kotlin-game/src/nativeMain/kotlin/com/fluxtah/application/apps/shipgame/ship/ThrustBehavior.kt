package com.fluxtah.application.apps.shipgame.ship

import com.fluxtah.application.api.*

class ThrustBehavior(private val isThrusting: () -> Boolean) : EntityBehavior {
    private val state = ThrustBehaviorState()

    lateinit var thrustSound: Sound

    override fun initialize(scene: Scene, entity: Entity) {
        thrustSound = scene.sounds["up-thrust"]!!
    }

    override fun beforeUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
        // We call native less often if we cache the result of isThrusting() before update() is called
        state.thrusting = isThrusting()
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        // Adjust thrust based on key input
        if (state.thrusting) {
            thrustSound.playIfNotPlaying()
            state.thrust = (state.thrust + state.thrustIncrement * fixedTimeStep).coerceAtMost(state.maxThrust)
        } else if (state.thrust > 0.0f) {
            thrustSound.stopIfPlaying()
            state.thrust = (state.thrust - state.thrustIncrement * fixedTimeStep).coerceAtLeast(0.0f)
        }

        // Calculate forces and acceleration
        val gravitationalForce = state.shipMass * state.gravity
        val netForce = state.thrust - gravitationalForce
        val acceleration = netForce / state.shipMass

        // Update velocity and position
        state.velocityY += acceleration * fixedTimeStep
        state.positionY += state.velocityY * fixedTimeStep

        // Apply max velocity
        state.velocityY = state.velocityY.coerceAtMost(state.maxVelocity)

        // When at ground level and not thrusting
        if (state.positionY <= 0.0f && !state.thrusting) {
            state.velocityY = 0.0f
        }

        // Apply damping if overshooting max altitude
        if (state.positionY > state.maxAltitude) {
            state.velocityY *= state.overshootDamping
            state.positionY = state.maxAltitude + (state.positionY - state.maxAltitude) * state.overshootDamping
        }

        // Apply hover damping for smooth hovering
        if (state.positionY >= state.maxAltitude && state.thrust > gravitationalForce) {
            state.thrust = gravitationalForce
            state.velocityY *= state.hoverDamping
        }

        // Ensure the spaceship doesn't go below the ground
        state.positionY = state.positionY.coerceAtLeast(0.0f)

        entity.position(y = state.positionY)
    }

//    override fun afterUpdate(scene: Scene, entity: Entity, time: Float, deltaTime: Float) {
//        println("\rState: $state")
//    }

    data class ThrustBehaviorState(
        var velocityY: Float = 0.0f,
        var positionY: Float = 0.0f,
        val gravity: Float = 2.0f,
        val shipMass: Float = 200.0f,
        var thrust: Float = 0.0f,
        val maxThrust: Float = 30000.0f,
        val maxAltitude: Float = 3.0f,
        val thrustIncrement: Float = 9000f,
        val overshootDamping: Float = 0.95f,
        val hoverDamping: Float = 0.99f,
        val maxVelocity: Float = 3.0f,
        var thrusting: Boolean = false,
    )
}