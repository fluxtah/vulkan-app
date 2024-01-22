package com.fluxtah.application.api.sequence

import com.fluxtah.application.api.scene.Scene

class SequenceBuilder {
    private val actions = mutableListOf<() -> Action>()

    fun build(): Sequence {
        return Sequence(actions)
    }

    /**
     * Wait for a duration of time before continuing to the next step in the sequence.
     */
    fun wait(duration: Float) {
        actions.add { Action.Wait(duration) }
    }

    fun action(block: (Scene) -> Unit) {
        actions.add { Action.InstantAction(block) }
    }
}

abstract class Action {
    abstract fun play(scene: Scene, time: Float, deltaTime: Float)
    abstract fun isComplete(): Boolean

    class Wait(private val duration: Float) : Action() {
        private var timeElapsed = 0.0f

        override fun play(scene: Scene, time: Float, deltaTime: Float) {
            timeElapsed += deltaTime
        }

        override fun isComplete(): Boolean {
            return timeElapsed >= duration
        }
    }

    class InstantAction(private val action: (Scene) -> Unit) : Action() {
        override fun play(scene: Scene, time: Float, deltaTime: Float) = action.invoke(scene)
        override fun isComplete(): Boolean = true
    }
}
