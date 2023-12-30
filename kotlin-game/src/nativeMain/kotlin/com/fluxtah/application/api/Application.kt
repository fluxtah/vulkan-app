package com.fluxtah.application.api

import com.fluxtah.application.MyApplication

interface Application {
    fun initialize()
    fun draw()
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
@CName("ktDrawApplication")
fun ktDrawApplication() {
    applicationInstance.draw()
}
