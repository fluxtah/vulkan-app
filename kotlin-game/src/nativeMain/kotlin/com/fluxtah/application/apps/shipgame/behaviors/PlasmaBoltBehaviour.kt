package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import kotlin.math.pow
import kotlin.math.sqrt

class PlasmaBoltBehaviour(val fireButtonPressed: () -> Boolean) : EntityBehavior {
    private var isBoltFiring = false
    private val direction = Vector3(0f, 0f, 1f)
    private val boltSpeed = 20.0f // Speed of the plasma bolt
    private val maxDistance = 10f // Define the maximum travel distance
    private lateinit var initialPosition: Vector3

    private lateinit var boltSound: Sound
    override fun initialize(scene: Scene, entity: Entity) {
        boltSound = scene.sounds["plasma-bolt"]!!
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        if (fireButtonPressed() && !isBoltFiring) {
            isBoltFiring = true
            initialPosition = Vector3(entity.positionX, entity.positionY, entity.positionZ)
            firePlasmaBolt(entity)
            boltSound.playIfNotPlaying()
        }

        if (isBoltFiring) {
            updatePlasmaBolt(entity)
        }
    }

    private fun firePlasmaBolt(entity: Entity) {
        println("Fire plasma bolt!")
        entity.position(0f, 0f, 0f)
    }

    private fun updatePlasmaBolt(entity: Entity) {
        // Apply speed to the movement
        val movementStep = boltSpeed * fixedTimeStep
        val newPosition = Vector3(
            entity.positionX + direction.x * movementStep,
            entity.positionY + direction.y * movementStep,
            entity.positionZ + direction.z * movementStep
        )
        entity.position(newPosition.x, newPosition.y, newPosition.z)

        // Check if the bolt has completed its firing cycle
        val distanceTraveled = distanceBetween(initialPosition, newPosition)
        if (distanceTraveled >= maxDistance) {
            isBoltFiring = false
        }
    }

    private fun distanceBetween(start: Vector3, end: Vector3): Float {
        return sqrt(
            (end.x - start.x).pow(2) +
                    (end.y - start.y).pow(2) +
                    (end.z - start.z).pow(2)
        )
    }
}

