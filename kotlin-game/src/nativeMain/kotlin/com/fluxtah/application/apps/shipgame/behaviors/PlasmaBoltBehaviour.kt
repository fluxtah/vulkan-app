package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.toRadians
import kotlin.math.cos
import kotlin.math.pow
import kotlin.math.sin
import kotlin.math.sqrt

class PlasmaBoltBehaviour(val fireButtonPressed: () -> Boolean) : EntityBehavior {
    private var isBoltFiring = false
    private val boltSpeed = 40.0f // Speed of the plasma bolt
    private val maxDistance = 20f // Define the maximum travel distance
    private var initialPosition: Vector3 = Vector3(0.0f, 0.0f, 0.0f)
    private var firingDirection: Vector3 = Vector3(0.0f, 0.0f, 0.0f)

    private lateinit var boltSound: Sound
    private lateinit var shipEntity: Entity

    override fun initialize(scene: Scene, entity: Entity) {
        boltSound = scene.sounds["plasma-bolt"]!!
        shipEntity = scene.entities["ship"]!!.entity
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        if (fireButtonPressed() && !isBoltFiring) {
            isBoltFiring = true
            initialPosition = Vector3(shipEntity.positionX, shipEntity.positionY, shipEntity.positionZ)
            firingDirection =
                calculateDirectionFromRotation(shipEntity.rotationX, shipEntity.rotationY, shipEntity.rotationZ)
            firePlasmaBolt(entity, initialPosition, firingDirection)
            boltSound.play()
        }

        if (isBoltFiring) {
            updatePlasmaBolt(entity, time)
        }
    }

    private fun firePlasmaBolt(entity: Entity, initialPosition: Vector3, firingDirection: Vector3) {
        println("Fire plasma bolt!")
        entity.setPosition(initialPosition.x, initialPosition.y, initialPosition.z)
        // Assuming your engine can directly set the direction
        entity.setRotation(shipEntity.rotationX, shipEntity.rotationY, shipEntity.rotationZ)
    }

    private fun updatePlasmaBolt(entity: Entity, time: Float) {
        val movementStep = boltSpeed * fixedTimeStep
        val newPosition = Vector3(
            entity.positionX + firingDirection.x * movementStep,
            entity.positionY + firingDirection.y * movementStep,
            entity.positionZ + firingDirection.z * movementStep
        )
        entity.setPosition(newPosition.x, newPosition.y, newPosition.z)

        val distanceTraveled = distanceBetween(initialPosition, newPosition)
        if (distanceTraveled >= maxDistance) {
            isBoltFiring = false
        }
    }

    private fun calculateDirectionFromRotation(rotationX: Float, rotationY: Float, rotationZ: Float): Vector3 {
        // Convert rotationY (yaw) to a unit direction vector
        // Assuming rotationY is in degrees and represents rotation around the Y-axis
        val radians = rotationY.toRadians()
        val dirX = sin(radians)
        val dirZ = cos(radians)

        // Assuming no pitch (rotationX) or roll (rotationZ) affects the direction
        // If they do, the calculation needs to be adjusted accordingly
        return Vector3(dirX, 0.0f, dirZ)
    }


    private fun distanceBetween(start: Vector3, end: Vector3): Float {
        return sqrt(
            (end.x - start.x).pow(2) +
                    (end.y - start.y).pow(2) +
                    (end.z - start.z).pow(2)
        )
    }
}
