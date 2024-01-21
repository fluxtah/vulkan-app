package com.fluxtah.application.api.emitter

import com.fluxtah.application.api.ApplicationContext
import com.fluxtah.application.api.interop.c_createEmitter
import com.fluxtah.application.api.interop.model.CreateEmitterInfo
import com.fluxtah.application.api.scene.EmitterInfo
import com.fluxtah.application.api.scene.EmitterPoolInfo
import com.fluxtah.application.api.scene.Scene
import com.fluxtah.application.api.scene.SceneDsl
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.cstr
import kotlinx.cinterop.memScoped

@SceneDsl
@OptIn(ExperimentalForeignApi::class)
class EmitterPoolBuilder(private val scene: Scene, private val id: String, private val modelPath: String) {
    private var maxParticles: Int = 100
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var rotationX: Float = 0.0f
    private var rotationY: Float = 0.0f
    private var rotationZ: Float = 0.0f
    private var scaleX: Float = 1.0f
    private var scaleY: Float = 1.0f
    private var scaleZ: Float = 1.0f

    private var initialSize: Int = 10
    private var startActive: Boolean = false

    private val behaviors = mutableListOf<() -> EmitterBehavior>()

    fun initialSize(size: Int) {
        initialSize = size
    }

    fun startActive() {
        startActive = true
    }

    fun maxParticles(size: Int) {
        maxParticles = size
    }

    fun position(x: Float = 0f, y: Float = 0f, z: Float = 0f) {
        positionX = x
        positionY = y
        positionZ = z
    }

    fun rotation(x: Float = 0f, y: Float = 0f, z: Float = 0f) {
        rotationX = x
        rotationY = y
        rotationZ = z
    }

    fun scale(x: Float = 1f, y: Float = 1f, z: Float = 1f) {
        scaleX = x
        scaleY = y
        scaleZ = z
    }

    fun behaviour(behavior: () -> EmitterBehavior) {
        behaviors.add(behavior)
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): EmitterPoolInfo {
        val initialEmitters = mutableListOf<EmitterInfo>().apply {
            repeat(initialSize) {
                add(createEmitterInfo())
            }
        }
        return EmitterPoolInfo(
            initialSize = initialSize,
            factory = { createEmitterInfo() },
            emittersAvailable = if (startActive) mutableListOf() else initialEmitters,
            emittersInUse = if (startActive) initialEmitters.onEach { it.emitter.inUse = true } else mutableListOf()
        )
    }

    private fun createEmitterInfo(): EmitterInfo {
        val cEmitter = memScoped {
            val info = cValue<CreateEmitterInfo> {
                modelFileName = modelPath.cstr.ptr
                maxParticles = this@EmitterPoolBuilder.maxParticles
                positionX = this@EmitterPoolBuilder.positionX
                positionY = this@EmitterPoolBuilder.positionY
                positionZ = this@EmitterPoolBuilder.positionZ
                rotationX = this@EmitterPoolBuilder.rotationX
                rotationY = this@EmitterPoolBuilder.rotationY
                rotationZ = this@EmitterPoolBuilder.rotationZ
                scaleX = this@EmitterPoolBuilder.scaleX
                scaleY = this@EmitterPoolBuilder.scaleY
                scaleZ = this@EmitterPoolBuilder.scaleX
            }
            c_createEmitter!!.invoke(ApplicationContext.vulcanContext!!, info.ptr)
        }

        val behaviors = behaviors.map { it().apply { this.scene = this@EmitterPoolBuilder.scene } }

        return EmitterInfo(
            emitter = Emitter(
                id = id,
                handle = cEmitter,
                initialPositionX = positionX,
                initialPositionY = positionY,
                initialPositionZ = positionZ,
                initialRotationX = rotationX,
                initialRotationY = rotationY,
                initialRotationZ = rotationZ,
                initialScaleX = scaleX,
                initialScaleY = scaleY,
                initialScaleZ = scaleZ,
                inUse = false,
                behaviors = behaviors
            ),
            behaviors = behaviors,
        )
    }
}