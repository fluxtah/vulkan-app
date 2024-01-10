package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.*
import com.fluxtah.application.apps.shipgame.Id

class FireWeaponBehaviour(val fireButtonPressed: () -> Boolean) : EntityBehavior {
    private var lastFireTime = 0.0f
    private val fireDelay = 0.3f

    private lateinit var boltSound: Sound

    override fun initialize(scene: Scene, entity: Entity) {
        boltSound = scene.soundById(Id.SOUND_PLASMA_BOLT)!!
    }

    override fun update(scene: Scene, entity: Entity, time: Float) {
        val canFireBolt = time - lastFireTime > fireDelay
        if (fireButtonPressed() && canFireBolt) {
            lastFireTime = time
            scene.entityFromPool(Id.ENT_PLASMA_BOLT) { bolt, behaviors ->
                boltSound.play()
                bolt.setPosition(entity.positionX, entity.positionY, entity.positionZ)
                bolt.setRotation(entity.rotationX, entity.rotationY, entity.rotationZ)
                bolt.visible = true
                behaviors.filterIsInstance<PlasmaBoltBehaviour2>().onEach {
                    it.fireBoltFrom(entity)
                }
            }
        }
    }
}
