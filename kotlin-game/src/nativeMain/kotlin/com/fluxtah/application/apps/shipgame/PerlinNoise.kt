package com.fluxtah.application.apps.shipgame

import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.floor
import kotlin.math.sin
import kotlin.random.Random

class PerlinNoise(seed: Int) {
    private val permutation = IntArray(512)
    private val gradients = Array(256) { Pair(0.0, 0.0) }

    init {
        val random = Random(seed)

        // Initialize gradients with uniform random unit vectors
        for (i in 0 until 256) {
            val angle = 2.0 * PI * random.nextDouble()
            gradients[i] = Pair(cos(angle), sin(angle))
        }

        // Initialize permutation table
        val p = (0 until 256).shuffled(random)
        for (i in 0 until 256) {
            permutation[i] = p[i]
            permutation[i + 256] = p[i]
        }
    }

    private fun lerp(a: Double, b: Double, t: Double): Double {
        return a + t * (b - a)
    }

    private fun fade(t: Double): Double {
        return t * t * t * (t * (t * 6 - 15) + 10)
    }

    private fun grad(hash: Int, x: Double, y: Double): Double {
        val h = hash and 7
        val u = if (h < 4) x else y
        val v = if (h < 4) y else x
        return (if ((h and 1) == 0) u else -u) + (if ((h and 2) == 0) v else -v)
    }

    fun generate(x: Double, y: Double): Double {
        val xi = floor(x).toInt() and 255
        val yi = floor(y).toInt() and 255
        val xf = x - floor(x)
        val yf = y - floor(y)
        val u = fade(xf)
        val v = fade(yf)

        val aa = permutation[permutation[xi] + yi]
        val ab = permutation[permutation[xi] + yi + 1]
        val ba = permutation[permutation[xi + 1] + yi]
        val bb = permutation[permutation[xi + 1] + yi + 1]

        val x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u)
        val x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u)

        return (lerp(x1, x2, v) + 1) / 2.0
    }
}
