package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.Extent2D
import com.fluxtah.application.api.interop.model.Viewport
import kotlinx.cinterop.CPointer
import kotlinx.cinterop.ExperimentalForeignApi

@OptIn(ExperimentalForeignApi::class)
typealias VkExtent2D = CPointer<Extent2D>

@OptIn(ExperimentalForeignApi::class)
typealias VkViewport = CPointer<Viewport>