package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.entity.Entity
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.distanceTo

class PlasmaBoltBehaviour(
    private val boltSpeed: Float = 40.0f,
    private val maxDistance: Float = 20f
) : EntityBehavior() {

    private var isBoltFiring: Boolean = false
    var initialPosition: Vector3 = Vector3(0.0f, 0.0f, 0.0f)
    var firingDirection: Vector3 = Vector3(0.0f, 0.0f, 0.0f)

    fun fireBolt() {
        isBoltFiring = true
    }

    override fun update(time: Float) {
        if (isBoltFiring) {
            updatePlasmaBolt()
        }
    }

    private fun updatePlasmaBolt() {
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
}
