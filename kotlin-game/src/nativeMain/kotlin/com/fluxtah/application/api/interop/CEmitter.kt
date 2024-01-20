package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.CreateEmitterInfo
import com.fluxtah.application.api.interop.model.EmitterArray
import com.fluxtah.application.api.scene.SceneImpl
import com.fluxtah.application.api.scene.activeSceneInfo
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CEmitter = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CCreateEmitterInfo = CPointer<CreateEmitterInfo>

@OptIn(ExperimentalForeignApi::class)
typealias CreateEmitterFunc = (CVulkanContext, CCreateEmitterInfo) -> CEmitter

@OptIn(ExperimentalForeignApi::class)
var c_createEmitter: CreateEmitterFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateEmitterFunc")
fun ktSetCreateEmitterFunc(callback: CPointer<CFunction<CreateEmitterFunc>>) {
    c_createEmitter = { context, info ->
        memScoped {
            callback.reinterpret<CFunction<CreateEmitterFunc>>()(
                context,
                info
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyEmitterFunc = (CVulkanContext, CEmitter) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroyEmitter: DestroyEmitterFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyEmitterFunc")
fun ktSetDestroyEmitterFunc(callback: CPointer<CFunction<(CVulkanContext, CEmitter) -> Unit>>) {
    c_destroyEmitter = { device, emitter ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CEmitter) -> Unit>>()(device, emitter)
        }
    }
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetEmitters")
fun ktGetEmitters(): CPointer<EmitterArray> {
    val scene = activeSceneInfo.scene as SceneImpl
    val emitters = scene.emitters.values.filter { it.emitter.visible }.map { it.emitter.handle } +
            scene.emitterPools.flatMap { it.value.emittersInUse.filter { it.emitter.visible }.map { it.emitter.handle } }

    val emitterPointerArray = nativeHeap.allocArray<COpaquePointerVar>(emitters.size)

    emitters.forEachIndexed { index, emitter ->
        emitterPointerArray[index] = emitter
    }

    val emitterArray = nativeHeap.alloc<EmitterArray>()
    emitterArray.emitters = emitterPointerArray
    emitterArray.size = emitters.size

    return emitterArray.ptr
}

@OptIn(ExperimentalForeignApi::class)
typealias EmitterPositionFunc = (CEmitter, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEmitterPosition: EmitterPositionFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPositionEmitterFunc")
fun ktSetPositionEmitterFunc(callback: CPointer<CFunction<(CEmitter, Float, Float, Float) -> Unit>>) {
    c_setEmitterPosition = { emitter, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<(CEmitter, Float, Float, Float) -> Unit>>()(
                emitter, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias EmitterRotationFunc = (CEmitter, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEmitterRotation: EmitterRotationFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetEmitterRotationFunc")
fun ktSetEmitterRotationFunc(callback: CPointer<CFunction<EmitterRotationFunc>>) {
    c_setEmitterRotation = { emitter, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<EmitterRotationFunc>>()(
                emitter, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias EmitterScaleFunc = (CEmitter, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEmitterScale: EmitterScaleFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetEmitterScaleFunc")
fun ktSetEmitterScaleFunc(callback: CPointer<CFunction<EmitterScaleFunc>>) {
    c_setEmitterScale = { emitter, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<EmitterScaleFunc>>()(
                emitter, x, y, z
            )
        }
    }
}
