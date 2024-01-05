package com.fluxtah.application.api

import com.fluxtah.application.api.interop.CLight
import com.fluxtah.application.api.interop.c_createLight
import com.fluxtah.application.api.interop.model.CreateLightInfo
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.memScoped

@OptIn(ExperimentalForeignApi::class)
class Light(val handle: CLight)

enum class LightType(val value: Int) {
    Point(0),
    Spot(1),
    Directional(2),
}

@SceneDsl
class LightBuilder {
    private var lightType = LightType.Directional
    private var colorR: Float = 1.0f
    private var colorG: Float = 1.0f
    private var colorB: Float = 1.0f
    private var colorA: Float = 1.0f
    private var positionX: Float = 0.0f
    private var positionY: Float = 0.0f
    private var positionZ: Float = 0.0f
    private var directionX: Float = 0.0f
    private var directionY: Float = 0.0f
    private var directionZ: Float = 0.0f
    private var intensity: Float = 1.0f

    fun type(type: LightType) {
        lightType = type
    }

    fun color(r: Float, g: Float, b: Float, a: Float) {
        colorR = r
        colorG = g
        colorB = b
        colorA = a
    }

    fun position(x: Float, y: Float, z: Float) {
        positionX = x
        positionY = y
        positionZ = z
    }

    fun direction(x: Float, y: Float, z: Float) {
        directionX = x
        directionY = y
        directionZ = z
    }

    fun intensity(intensity: Float) {
        this.intensity = intensity
    }

    @OptIn(ExperimentalForeignApi::class)
    fun build(): Light {
        val info = cValue<CreateLightInfo> {
            type = this@LightBuilder.lightType.value
            colorR = this@LightBuilder.colorR
            colorG = this@LightBuilder.colorG
            colorB = this@LightBuilder.colorB
            colorA = this@LightBuilder.colorA
            positionX = this@LightBuilder.positionX
            positionY = this@LightBuilder.positionY
            positionZ = this@LightBuilder.positionZ
            directionX = this@LightBuilder.directionX
            directionY = this@LightBuilder.directionY
            directionZ = this@LightBuilder.directionZ
            intensity = this@LightBuilder.intensity
        }

        val cLight = memScoped {
            c_createLight!!.invoke(info.ptr)
        }

        return Light(cLight)
    }
}