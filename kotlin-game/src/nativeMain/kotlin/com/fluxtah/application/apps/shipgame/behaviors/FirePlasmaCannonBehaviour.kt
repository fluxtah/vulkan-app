package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.apps.shipgame.Id

class FirePlasmaCannonBehaviour(private val fireDelay: Float = 0.3f, val fireButtonPressed: () -> Boolean) : EntityBehavior {
    private var lastFireTime = 0.0f
    private lateinit var boltSound: Sound

    override fun initialize(scene: Scene, entity: Entity) {
        boltSound = scene.soundById(Id.SOUND_PLASMA_BOLT)!!
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        val canFireBolt = time - lastFireTime > fireDelay
        if (fireButtonPressed() && canFireBolt) {
            lastFireTime = time
            scene.entityFromPool(Id.ENT_PLASMA_BOLT) { bolt, behaviors ->
                bolt.setPosition(entity.positionX, entity.positionY, entity.positionZ)
                bolt.setRotation(entity.rotationX, entity.rotationY, entity.rotationZ)
                bolt.visible = true
                boltSound.play()
                behaviors.filterIsInstance<PlasmaBoltBehaviour>().onEach {
                    it.initialPosition.x = entity.positionX
                    it.initialPosition.y = entity.positionY
                    it.initialPosition.z = entity.positionZ
                    it.firingDirection = Vector3.calculateDirectionFromYaw(entity.rotationY)
                    it.fireBolt()
                }
            }
        }
    }
}
