package com.fluxtah.application.api.emitter

import com.fluxtah.application.api.ApplicationContext
import com.fluxtah.application.api.interop.c_createEmitter
import com.fluxtah.application.api.interop.model.CreateEmitterInfo
import com.fluxtah.application.api.scene.EmitterInfo
import com.fluxtah.application.api.scene.Scene
import com.fluxtah.application.api.scene.SceneDsl
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.cstr
import kotlinx.cinterop.memScoped

@SceneDsl
@OptIn(ExperimentalForeignApi::class)
class EmitterBuilder(private val scene: Scene, private val id: String, private val modelPath: String) {
    private var maxParticles: Int = 100
    private var particleBatchSize: Int = 0

    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var rotationX: Float = 0.0f
    private var rotationY: Float = 0.0f
    private var rotationZ: Float = 0.0f
    private var scaleX: Float = 1.0f
    private var scaleY: Float = 1.0f
    private var scaleZ: Float = 1.0f

    private var computeShaderPath: String? = null
    private var vertexShaderPath: String? = null
    private var fragmentShaderPath: String? = null

    private val behaviors = mutableListOf<() -> EmitterBehavior>()

    fun maxParticles(size: Int) {
        maxParticles = size
    }

    fun particleBatchSize(size: Int) {
        particleBatchSize = size
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

    fun computeShader(path: String) {
        computeShaderPath = path
    }

    fun vertexShader(path: String) {
        vertexShaderPath = path
    }

    fun fragmentShader(path: String) {
        fragmentShaderPath = path
    }

    fun behaviour(behavior: () -> EmitterBehavior) {
        behaviors.add(behavior)
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): EmitterInfo = createEmitterInfo()

    private fun createEmitterInfo(): EmitterInfo {

        val cEmitter = memScoped {
            val info = cValue<CreateEmitterInfo> {
                modelFileName = modelPath.cstr.ptr
                computeShaderFileName = computeShaderPath?.cstr?.ptr
                vertexShaderFileName = vertexShaderPath?.cstr?.ptr
                fragmentShaderFileName = fragmentShaderPath?.cstr?.ptr
                maxParticles = this@EmitterBuilder.maxParticles
                particleBatchSize = this@EmitterBuilder.particleBatchSize
                positionX = this@EmitterBuilder.positionX
                positionY = this@EmitterBuilder.positionY
                positionZ = this@EmitterBuilder.positionZ
                rotationX = this@EmitterBuilder.rotationX
                rotationY = this@EmitterBuilder.rotationY
                rotationZ = this@EmitterBuilder.rotationZ
                scaleX = this@EmitterBuilder.scaleX
                scaleY = this@EmitterBuilder.scaleY
                scaleZ = this@EmitterBuilder.scaleX
            }
            c_createEmitter!!.invoke(ApplicationContext.vulcanContext!!, info.ptr)
        }

        val behaviors = behaviors.map { it().apply { this.scene = this@EmitterBuilder.scene } }

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
                inUse = true,
                behaviors = behaviors
            ),
            behaviors = behaviors,
        )
    }
}