package com.fluxtah.application.api.math

import kotlin.math.PI

fun Float.toRadians() : Float {
    return this * (PI / 180.0).toFloat()
}

fun Double.toRadians() : Double {
    return this * (PI / 180.0).toFloat()
}