package com.fluxtah.application.api.entity

import com.fluxtah.application.api.EntityBehavior
import com.fluxtah.application.api.interop.*
import kotlinx.cinterop.*
import kotlin.reflect.KClass

@OptIn(ExperimentalForeignApi::class)
class Entity(
    val id: String,
    val handle: CEntity,
    initialPositionX: Float = 0.0f,
    initialPositionY: Float = 0.0f,
    initialPositionZ: Float = 0.0f,
    initialRotationX: Float = 0.0f,
    initialRotationY: Float = 0.0f,
    initialRotationZ: Float = 0.0f,
    initialScaleX: Float = 1.0f,
    initialScaleY: Float = 1.0f,
    initialScaleZ: Float = 1.0f,
    var inUse: Boolean = true,
    var visible: Boolean = true,
    val behaviors: List<EntityBehavior>
) {
    init {
        behaviors.forEach { it.entity = this }
    }

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

    private var _scaleX: Float = initialScaleX
    val scaleX: Float
        get() {
            return _scaleX
        }

    private var _scaleY: Float = initialScaleY
    val scaleY: Float
        get() {
            return _scaleY
        }

    private var _scaleZ: Float = initialScaleZ
    val scaleZ: Float
        get() {
            return _scaleZ
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

    fun setScale(x: Float? = null, y: Float? = null, z: Float? = null) {
        _scaleX = x ?: _scaleX
        _scaleY = y ?: _scaleY
        _scaleZ = z ?: _scaleZ
        c_setEntityScale!!.invoke(handle, _scaleX, _scaleY, _scaleZ)
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

    fun resetBehaviors() {
        behaviors.forEach { it.reset() }
    }

    inline fun <reified T : EntityBehavior> getBehaviorByType(): T {
        for (behavior in behaviors) {
            if (behavior is T) {
                return behavior
            }
        }
        throw RuntimeException("Entity does not have a behavior of type ${T::class.simpleName}")
    }
}

