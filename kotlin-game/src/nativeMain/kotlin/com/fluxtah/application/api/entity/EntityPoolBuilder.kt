package com.fluxtah.application.api.entity

import com.fluxtah.application.api.*
import com.fluxtah.application.api.interop.c_attachKotlinEntity
import com.fluxtah.application.api.interop.c_createEntity
import com.fluxtah.application.api.interop.model.CreateEntityInfo
import com.fluxtah.application.api.scene.*
import kotlinx.cinterop.*

@SceneDsl
@OptIn(ExperimentalForeignApi::class)
class EntityPoolBuilder(private val scene: Scene, private val id: String, private val modelPath: String) {
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var rotationX: Float = 0.0f
    private var rotationY: Float = 0.0f
    private var rotationZ: Float = 0.0f
    private var scaleX: Float = 1.0f
    private var scaleY: Float = 1.0f
    private var scaleZ: Float = 1.0f

    private var orientatedBoundingBox: Boolean = false

    private var onCollision: OnCollision? = null

    private var initialSize: Int = 10
    private var startActive: Boolean = false

    private val behaviors = mutableListOf<() -> EntityBehavior>()

    fun initialSize(size: Int) {
        initialSize = size
    }

    fun startActive() {
        startActive = true
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

    fun useOrientedBoundingBox() {
        orientatedBoundingBox = true
    }

    fun onCollision(block: OnCollision) {
        onCollision = block
    }

    fun behaviour(behavior: () -> EntityBehavior) {
        behaviors.add(behavior)
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): EntityPoolInfo {
        val initialEntities = mutableListOf<EntityInfo>().apply {
            repeat(initialSize) {
                add(createEntityInfo())
            }
        }
        return EntityPoolInfo(
            initialSize = initialSize,
            factory = { createEntityInfo() },
            entitiesAvailable = if (startActive) mutableListOf() else initialEntities,
            entitiesInUse = if (startActive) initialEntities.onEach { it.entity.inUse = true } else mutableListOf()
        )
    }

    private fun createEntityInfo(): EntityInfo {
        val info = cValue<CreateEntityInfo> {
            positionX = this@EntityPoolBuilder.positionX
            positionY = this@EntityPoolBuilder.positionY
            positionZ = this@EntityPoolBuilder.positionZ
            rotationX = this@EntityPoolBuilder.rotationX
            rotationY = this@EntityPoolBuilder.rotationY
            rotationZ = this@EntityPoolBuilder.rotationZ
            scaleX = this@EntityPoolBuilder.scaleX
            scaleY = this@EntityPoolBuilder.scaleY
            scaleZ = this@EntityPoolBuilder.scaleX
            useOrientedBoundingBox = this@EntityPoolBuilder.orientatedBoundingBox
        }

        val cEntity = memScoped {
            c_createEntity!!.invoke(ApplicationContext.vulcanContext!!, modelPath.cstr.ptr, info.ptr)
        }

        val behaviors = behaviors.map { it().apply { this.scene = this@EntityPoolBuilder.scene } }

        return EntityInfo(
            entity = Entity(
                id = id,
                handle = cEntity,
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
            onCollision = onCollision
        ).apply {
            val ref = StableRef.create(this)
            c_attachKotlinEntity!!.invoke(cEntity, ref.asCPointer())
            stableRef = ref
        }
    }
}