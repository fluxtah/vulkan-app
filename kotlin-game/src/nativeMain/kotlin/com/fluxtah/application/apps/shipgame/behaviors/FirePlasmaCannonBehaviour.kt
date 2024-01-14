package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.entity.Entity
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.apps.shipgame.Id

class FirePlasmaCannonBehaviour(private val fireDelay: Float = 0.3f, val fireButtonPressed: () -> Boolean) :
    EntityBehavior() {
    private var lastFireTime = 0.0f
    private lateinit var boltSound: Sound

    override fun initialize() {
        boltSound = scene.soundById(Id.SOUND_PLASMA_BOLT)!!
    }

    override fun update(time: Float) {
        val canFireBolt = time - lastFireTime > fireDelay
        if (fireButtonPressed() && canFireBolt) {
            lastFireTime = time
            scene.entityFromPool(Id.ENT_PLASMA_BOLT) { bolt ->
                val boltBehaviour = bolt.getBehaviorByType<PlasmaBoltBehaviour>()
                bolt.setPosition(entity.positionX, entity.positionY, entity.positionZ)
                bolt.setRotation(entity.rotationX, entity.rotationY, entity.rotationZ)
                bolt.visible = true
                boltSound.play()
                boltBehaviour.initialPosition.x = entity.positionX
                boltBehaviour.initialPosition.y = entity.positionY
                boltBehaviour.initialPosition.z = entity.positionZ
                boltBehaviour.firingDirection = Vector3.calculateDirectionFromYaw(entity.rotationY)
                boltBehaviour.fireBolt()
            }
        }
    }
}
