package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias IsKeyPressedFunc = (Int) -> Int

@OptIn(ExperimentalForeignApi::class)
var c_isKeyPressed: IsKeyPressedFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetIsKeyPressedFunc")
fun ktSetIsKeyPressedFunc(callback: CPointer<CFunction<(Int) -> Int>>) {
    c_isKeyPressed = { key ->
        memScoped {
            callback.reinterpret<CFunction<(Int) -> Int>>()(
                key
            )
        }
    }
}
