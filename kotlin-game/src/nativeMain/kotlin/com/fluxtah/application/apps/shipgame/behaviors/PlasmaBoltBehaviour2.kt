package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.distanceTo
import com.fluxtah.application.api.math.toRadians
import kotlin.math.cos
import kotlin.math.pow
import kotlin.math.sin
import kotlin.math.sqrt

class PlasmaBoltBehaviour2 : EntityBehavior {
    private var isBoltFiring = false
    private val boltSpeed = 40.0f // Speed of the plasma bolt
    private val maxDistance = 20f // Define the maximum travel distance
    var initialPosition: Vector3 = Vector3(0.0f, 0.0f, 0.0f)
    var firingDirection: Vector3 = Vector3(0.0f, 0.0f, 0.0f)

    fun fireBoltFrom(sourceEntity: Entity) {
        isBoltFiring = true
        initialPosition = Vector3(sourceEntity.positionX, sourceEntity.positionY, sourceEntity.positionZ)
        firingDirection =
            calculateDirectionFromRotation(sourceEntity.rotationX, sourceEntity.rotationY, sourceEntity.rotationZ)
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        if (isBoltFiring) {
            updatePlasmaBolt(scene, entity)
        }
    }

    private fun updatePlasmaBolt(scene: Scene, entity: Entity) {
        val movementStep = boltSpeed * fixedTimeStep
        val newPosition = Vector3(
            entity.positionX + firingDirection.x * movementStep,
            entity.positionY + firingDirection.y * movementStep,
            entity.positionZ + firingDirection.z * movementStep
        )
        entity.setPosition(newPosition.x, newPosition.y, newPosition.z)

        val distanceTraveled = initialPosition.distanceTo(newPosition)
        if (distanceTraveled >= maxDistance) {
            isBoltFiring = false
            entity.visible = false
            scene.entityToPool(entity)
        }
    }

    private fun calculateDirectionFromRotation(rotationX: Float, rotationY: Float, rotationZ: Float): Vector3 {
        // Convert rotationY (yaw) to a unit direction vector
        // Assuming rotationY is in degrees and represents rotation around the Y-axis
        val radians = rotationY.toRadians()
        val dirX = sin(radians)
        val dirZ = cos(radians)

        return Vector3(dirX, 0.0f, dirZ)
    }
}
