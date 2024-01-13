package com.fluxtah.application.api.math

import kotlin.math.cos
import kotlin.math.pow
import kotlin.math.sin
import kotlin.math.sqrt

data class Vector3(var x: Float = 0f, var y: Float = 0f, var z: Float = 0f) {
    companion object {
        val zero = Vector3(0f, 0f, 0f)

        // Linear interpolation (lerp) function
        fun lerp(a: Vector3, b: Vector3, t: Float): Vector3 = Vector3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
        )

        fun distanceBetween(start: Vector3, end: Vector3): Float = sqrt(
            (end.x - start.x).pow(2) +
                    (end.y - start.y).pow(2) +
                    (end.z - start.z).pow(2)
        )

        fun calculateDirectionFromYaw(rotationY: Float): Vector3 {
            // Convert rotationY (yaw) to radians
            val radians = rotationY.toRadians()

            // Calculate direction vector using yaw
            val dirX = sin(radians)
            val dirZ = cos(radians)

            return Vector3(dirX, 0.0f, dirZ)
        }
    }

    operator fun minus(other: Vector3) = Vector3(x - other.x, y - other.y, z - other.z)
    operator fun plus(other: Vector3) = Vector3(x + other.x, y + other.y, z + other.z)
    operator fun times(value: Float) = Vector3(x * value, y * value, z * value)
}

fun Vector3.lerp(target: Vector3, alpha: Float) {
    x = x * (1 - alpha) + target.x * alpha
    y = y * (1 - alpha) + target.y * alpha
    z = z * (1 - alpha) + target.z * alpha
}

fun Vector3.distanceTo(end: Vector3): Float = sqrt(
    (end.x - x).pow(2) +
            (end.y - y).pow(2) +
            (end.z - z).pow(2)
)

fun Vector3.negate() {
    x = -x
    y = -y
    z = -z
}

fun Vector3.length(): Float = sqrt(x * x + y * y + z * z)