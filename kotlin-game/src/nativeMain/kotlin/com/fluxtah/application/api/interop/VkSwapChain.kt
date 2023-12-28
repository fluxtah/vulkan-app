package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.Extent2D
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkSwapChain = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private var vkSwapChain: VkSwapChain? = null
@OptIn(ExperimentalForeignApi::class)
private var swapChainExtent: CPointer<Extent2D>? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVkSwapChain")
fun ktSetVkSwapChain(swapChain: VkSwapChain) {
    vkSwapChain = swapChain
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkSwapChain")
fun ktGetVkSwapChain(): VkSwapChain {
    return vkSwapChain!!
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetSwapChainExtent")
fun ktSetSwapChainExtent(extent: CPointer<Extent2D>) {
    swapChainExtent = extent
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetSwapChainExtent")
fun ktGetSwapChainExtent(): CPointer<Extent2D>? {
    return swapChainExtent!!
}
