package com.fluxtah.application.api.math

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