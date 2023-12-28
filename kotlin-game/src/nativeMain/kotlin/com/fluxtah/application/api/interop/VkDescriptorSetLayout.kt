package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias VkDescriptorSetLayout = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
private var vkDescriptorSetLayout: VkDescriptorSetLayout? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetVkDescriptorSetLayout")
fun ktSetVkDescriptorSetLayout(descriptorSetLayout: VkDescriptorSetLayout) {
    vkDescriptorSetLayout = descriptorSetLayout
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetVkDescriptorSetLayout")
fun ktGetVkDescriptorSetLayout(): VkDescriptorSetLayout {
    return vkDescriptorSetLayout!!
}
