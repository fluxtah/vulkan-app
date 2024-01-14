package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.apps.shipgame.Id

class AsteroidDieBehavior : EntityBehavior() {
    fun die() {
        scene.entityToPool(entity)
        scene.soundById(Id.SOUND_ASTEROID_EXPLODE)?.play()
        entity.visible = false
    }
}
