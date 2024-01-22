package com.fluxtah.application.apps.shipgame.behaviors

import com.fluxtah.application.api.camera.CameraBehavior
import com.fluxtah.application.api.entity.Entity
import com.fluxtah.application.api.fixedTimeStep
import com.fluxtah.application.api.math.Vector3
import com.fluxtah.application.api.math.lerp
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.sin

class ChaseCameraBehavior(
    private var targetEntityId: String,
    private var offset: Vector3 = Vector3(0f, 3f, -4.2f),
    private var position: Vector3 = Vector3(),
    private var smoothingFactor: Float = 8.0f // Adjust for desired smoothing
) : CameraBehavior() {

    private lateinit var target: Entity

    override fun initialize() {
        target = scene.entityById(targetEntityId) ?: throw Exception("Target entity not found")
        camera.lookAt(0f, 0f, 0f)
    }

    override fun update(time: Float) {
        // Calculate the offset position based on target's rotation
        val rotatedOffset = rotateOffsetByTargetYaw(offset, -target.rotationY)

        // Calculate desired camera position based on target's position and rotated offset
        val desiredPosition = Vector3(target.positionX, target.positionY, target.positionZ) + rotatedOffset

        // Add smoothing/lerping for smoother camera movement
        position.lerp(desiredPosition, fixedTimeStep * smoothingFactor)

        // Update the camera's position
        setCameraPosition(position)
    }

    private fun rotateOffsetByTargetYaw(offset: Vector3, yaw: Float): Vector3 {
        // Convert yaw to radians as most math libraries use radians
        val yawRadians = yaw * (PI / 180.0).toFloat()
        val cosYaw = cos(yawRadians)
        val sinYaw = sin(yawRadians)

        // Rotate the offset around the Y axis
        return Vector3(
            offset.x * cosYaw - offset.z * sinYaw,
            offset.y,
            offset.x * sinYaw + offset.z * cosYaw
        )
    }

    private fun setCameraPosition(pos: Vector3) {
        // Set your camera's position here
        camera.position(pos.x, pos.y, pos.z)
        camera.lookAt(target.positionX, target.positionY, target.positionZ)
        camera.pitch(20f)
        camera.applyChanges()
    }
}
