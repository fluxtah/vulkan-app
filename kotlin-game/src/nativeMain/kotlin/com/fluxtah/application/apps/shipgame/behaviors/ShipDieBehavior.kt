package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.entity.EntityBehavior
import com.fluxtah.application.apps.shipgame.Id

class ShipDieBehavior : EntityBehavior() {
    fun die() {
        if (entity.inUse) {
            scene.soundById(Id.SOUND_ASTEROID_EXPLODE)?.play()
            entity.visible = false
            entity.inUse = false
            scene.emitterFromPool(Id.EMITTER_EXPLOSION) {
                it.setPosition(entity.positionX, entity.positionY, entity.positionZ)
                val behavior = it.getBehaviorByType<ExplosionEmitterBehavior>()
                behavior.explode()
            }
        }
    }
}
