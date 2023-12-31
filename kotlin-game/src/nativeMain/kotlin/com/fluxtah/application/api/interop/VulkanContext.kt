package com.fluxtah.application.api.interop

import com.fluxtah.application.api.ApplicationContext
import kotlinx.cinterop.CPointed
import kotlinx.cinterop.CPointer
import kotlinx.cinterop.ExperimentalForeignApi
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CVulkanContext = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVulkanContext")
fun ktSetVulkanContext(context: CVulkanContext) {
    ApplicationContext.vulcanContext = context
}
