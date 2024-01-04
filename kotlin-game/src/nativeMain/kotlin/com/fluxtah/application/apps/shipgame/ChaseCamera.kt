package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.Camera
import com.fluxtah.application.api.Entity

class ChaseCamera(val camera: Camera, val target: Entity) {
    private var position: Vector3 = Vector3()
    private var offset: Vector3 = Vector3(0f, 5f, -5f) // Adjust for desired camera position
    private var smoothingFactor = 0.8f // Adjust for desired smoothing

    fun update(deltaTime: Float) {
        // Calculate desired camera position based on target's position and offset
        val desiredPosition = Vector3(target.positionX, target.positionY, target.positionZ) + offset

        // Optionally, add smoothing/lerping for smoother camera movement
        position.lerp(desiredPosition, deltaTime * smoothingFactor)

        // Update the camera's position
        setCameraPosition(position)
    }

    private fun setCameraPosition(pos: Vector3) {
        // Set your camera's position here
        camera.position(pos.x, pos.y, pos.z)
        camera.lookAt(target.positionX, target.positionY, target.positionZ)
        camera.applyChanges()
    }
}

data class Vector3(var x: Float = 0f, var y: Float = 0f, var z: Float = 0f) {
    companion object {
        val zero = Vector3(0f, 0f, 0f)

        // Linear interpolation (lerp) function
        fun lerp(a: Vector3, b: Vector3, t: Float): Vector3 {
            return Vector3(
                a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t
            )
        }
    }

    // Existing operator overloads
    operator fun minus(other: Vector3) = Vector3(x - other.x, y - other.y, z - other.z)
    operator fun plus(other: Vector3) = Vector3(x + other.x, y + other.y, z + other.z)
    operator fun times(value: Float) = Vector3(x * value, y * value, z * value)
}

fun Vector3.lerp(target: Vector3, alpha: Float) {
    x = x * (1 - alpha) + target.x * alpha
    y = y * (1 - alpha) + target.y * alpha
    z = z * (1 - alpha) + target.z * alpha
}