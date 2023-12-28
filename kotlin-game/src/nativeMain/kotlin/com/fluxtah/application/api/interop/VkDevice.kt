package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkDevice = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private var vkDevice: VkDevice? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVkDevice")
fun ktSetVkDevice(device: VkDevice) {
    vkDevice = device
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkDevice")
fun ktGetVkDevice(): VkDevice {
    return vkDevice!!
}
