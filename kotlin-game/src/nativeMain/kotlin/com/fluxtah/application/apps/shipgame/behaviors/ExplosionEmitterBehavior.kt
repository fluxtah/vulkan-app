package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.emitter.EmitterBehavior
import com.fluxtah.application.api.fixedTimeStep

class ExplosionEmitterBehavior : EmitterBehavior() {
    private var timeToLive = 0.0f;

    fun explode() {
        timeToLive = 1.5f
        emitter.reset();
        emitter.visible = true
    }
    override fun update(time: Float) {
        super.update(time)
        if(timeToLive > 0.0f) {
            timeToLive -= fixedTimeStep
            if(timeToLive <= 0.0f) {
                emitter.visible = false
                scene.emitterToPool(emitter)
            }
        }
    }
}