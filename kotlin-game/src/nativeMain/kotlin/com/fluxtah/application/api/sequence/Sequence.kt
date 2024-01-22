package com.fluxtah.application.api.sequence

import com.fluxtah.application.api.scene.Scene
import com.fluxtah.application.api.scene.SceneImpl

class Sequence(private val actions: MutableList<() -> Action>) {
    lateinit var scene: Scene

    private var actionIndex = 0
    private var currentAction: Action? = null

    fun play() {
        (scene as SceneImpl).dispatchSequence(this)
    }

    fun advance(time: Float, deltaTime: Float) {
        if (currentAction == null) {
            currentAction = actions[actionIndex].invoke()
        }

        currentAction?.play(scene, time, deltaTime)

        if (currentAction?.isComplete() == true) {
            actionIndex++
            currentAction = actions.getOrNull(actionIndex)?.invoke()

            if (currentAction == null) {
                (scene as SceneImpl).stopSequence(this)
            }
        }
    }

    fun reset() {
        (scene as SceneImpl).stopSequence(this)
        actionIndex = 0
        currentAction = null
    }
}