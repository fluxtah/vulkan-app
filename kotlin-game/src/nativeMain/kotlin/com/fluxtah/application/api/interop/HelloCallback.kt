package com.fluxtah.application.api.interop

import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

typealias HelloFunc = (String) -> Unit

var helloFuncCallback: HelloFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("helloCallback")
fun helloCallback(callback: CPointer<CFunction<(CPointer<ByteVar>) -> Unit>>) {
    helloFuncCallback = { message ->
        memScoped {
            callback.reinterpret<CFunction<(CPointer<ByteVar>) -> Unit>>()(message.cstr.ptr)
        }
    }
}