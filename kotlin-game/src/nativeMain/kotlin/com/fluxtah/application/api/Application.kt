package com.fluxtah.application.api

import com.fluxtah.application.MyApplication
import com.fluxtah.application.api.input.Key

interface Application {
    fun initialize()
    fun update(time: Float, deltaTime: Float) {}
}

fun Application.isKeyPressed(key: Key) : Boolean {
    TODO("Not yet implemented")
}

private lateinit var applicationInstance: Application

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktCreateApplication")
fun ktCreateApplication() {
    applicationInstance = MyApplication()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktInitApplication")
fun ktInitApplication() {
    applicationInstance.initialize()
}

@OptIn(kotlin.experimental.ExperimentalNativeApi::class)
@CName("ktUpdateApplication")
fun ktUpdateApplication(time: Float, deltaTime: Float) {
    applicationInstance.update(time, deltaTime)
}
