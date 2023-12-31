package com.fluxtah.application.api

class Light

@SceneDsl
class LightBuilder {
    fun color(r: Float, g: Float, b: Float, a: Float) { /* ... */
    }

    fun position(x: Float, y: Float, z: Float) { /* ... */
    }

    fun build(): Light {
        return Light()
    }
}