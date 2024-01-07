package com.fluxtah.application.api

import com.fluxtah.application.api.interop.*
import com.fluxtah.application.api.interop.model.CreateEntityInfo
import kotlinx.cinterop.*

@OptIn(ExperimentalForeignApi::class)
class Entity(
    val handle: CEntity,
    initialPositionX: Float = 0.0f,
    initialPositionY: Float = 0.0f,
    initialPositionZ: Float = 0.0f,
    initialRotationX: Float = 0.0f,
    initialRotationY: Float = 0.0f,
    initialRotationZ: Float = 0.0f,
) {
    var visible: Boolean = true
    
    private var _positionX: Float = initialPositionX
    val positionX: Float
        get() {
            return _positionX
        }

    private var _positionY: Float = initialPositionY
    val positionY: Float
        get() {
            return _positionY
        }

    private var _positionZ: Float = initialPositionZ
    val positionZ: Float
        get() {
            return _positionZ
        }

    private var _rotationX: Float = initialRotationX
    val rotationX: Float
        get() {
            return _rotationX
        }

    private var _rotationY: Float = initialRotationY
    val rotationY: Float
        get() {
            return _rotationY
        }

    private var _rotationZ: Float = initialRotationZ
    val rotationZ: Float
        get() {
            return _rotationZ
        }

    fun setPosition(x: Float? = null, y: Float? = null, z: Float? = null) {
        _positionX = x ?: _positionX
        _positionY = y ?: _positionY
        _positionZ = z ?: _positionZ
        c_setEntityPosition!!.invoke(handle, _positionX, _positionY, _positionZ)
    }

    fun setRotation(x: Float? = null, y: Float? = null, z: Float? = null) {
        _rotationX = x ?: _rotationX
        _rotationY = y ?: _rotationY
        _rotationZ = z ?: _rotationZ
        c_setEntityRotation!!.invoke(handle, _rotationX, _rotationY, _rotationZ)
    }

    fun rotate(x: Float = 0f, y: Float = 0f, z: Float = 0f) {
        _rotationX += x
        _rotationY += y
        _rotationZ += z
        c_setEntityRotation!!.invoke(handle, _rotationX, _rotationY, _rotationZ)
    }

    fun translate(x: Float = 0.0f, y: Float = 0.0f, z: Float = 0.0f) {
        _positionX += x
        _positionY += y
        _positionZ += z
        c_setEntityPosition!!.invoke(handle, _positionX, _positionY, _positionZ)
    }
}

@SceneDsl
class EntityBuilder(private val modelPath: String) {
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var rotationX: Float = 0.0f
    private var rotationY: Float = 0.0f
    private var rotationZ: Float = 0.0f
    private var scaleX: Float = 1.0f
    private var scaleY: Float = 1.0f
    private var scaleZ: Float = 1.0f

    private var onSceneEntityUpdate: OnSceneEntityUpdate? = null
    private var onSceneBeforeEntityUpdate: OnSceneBeforeEntityUpdate? = null
    private var onSceneAfterEntityUpdate: OnSceneAfterEntityUpdate? = null

    private val behaviors = mutableListOf<EntityBehavior>()

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

    fun onSceneUpdate(block: OnSceneEntityUpdate) {
        onSceneEntityUpdate = block
    }

    fun onSceneBeforeUpdate(block: OnSceneBeforeEntityUpdate) {
        onSceneBeforeEntityUpdate = block
    }

    fun onSceneAfterUpdate(block: OnSceneAfterEntityUpdate) {
        onSceneAfterEntityUpdate = block
    }

    fun behaviour(behavior: EntityBehavior) {
        behaviors.add(behavior)
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): EntityInfo {
        val info = cValue<CreateEntityInfo> {
            positionX = this@EntityBuilder.positionX
            positionY = this@EntityBuilder.positionY
            positionZ = this@EntityBuilder.positionZ
            rotationX = this@EntityBuilder.rotationX
            rotationY = this@EntityBuilder.rotationY
            rotationZ = this@EntityBuilder.rotationZ
            scaleX = this@EntityBuilder.scaleX
            scaleY = this@EntityBuilder.scaleY
            scaleZ = this@EntityBuilder.scaleX
        }

        val cEntity = memScoped {
            c_createEntity!!.invoke(ApplicationContext.vulcanContext!!, modelPath.cstr.ptr, info.ptr)
        }

        return EntityInfo(
            entity = Entity(
                handle = cEntity,
                initialPositionX = positionX,
                initialPositionY = positionY,
                initialPositionZ = positionZ
            ),
            onSceneEntityUpdate = onSceneEntityUpdate,
            onSceneBeforeEntityUpdate = onSceneBeforeEntityUpdate,
            onSceneAfterEntityUpdate = onSceneAfterEntityUpdate,
            behaviors = behaviors
        )
    }
}
