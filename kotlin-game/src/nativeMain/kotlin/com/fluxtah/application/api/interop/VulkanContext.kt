package com.fluxtah.application.api.interop

import kotlinx.cinterop.CPointed
import kotlinx.cinterop.CPointer
import kotlinx.cinterop.ExperimentalForeignApi
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VulkanContext = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private var vulcanContext: VulkanContext? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVulkanContext")
fun ktSetVulkanContext(context: VulkanContext) {
    vulcanContext = context
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVulkanContext")
fun ktGetVulkanContext(): VulkanContext {
    return vulcanContext!!
}
