package com.fluxtah.application.api

import com.fluxtah.application.api.interop.CVulkanContext
import kotlinx.cinterop.ExperimentalForeignApi

object ApplicationContext {
    @OptIn(ExperimentalForeignApi::class)
    internal var vulcanContext: CVulkanContext? = null
}