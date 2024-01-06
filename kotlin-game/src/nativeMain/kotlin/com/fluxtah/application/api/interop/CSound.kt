package com.fluxtah.application.api.interop

import com.fluxtah.application.api.interop.model.CreateSoundInfo
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CSound = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CCreateSoundInfo = CPointer<CreateSoundInfo>

@OptIn(ExperimentalForeignApi::class)
typealias LoadSoundFunc = (CPointer<ByteVar>, CCreateSoundInfo) -> CSound

@OptIn(ExperimentalForeignApi::class)
var c_loadSound: LoadSoundFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetLoadSoundFunc")
fun ktSetLoadSoundFunc(callback: CPointer<CFunction<LoadSoundFunc>>) {
    c_loadSound = { name, info ->
        memScoped {
            callback.reinterpret<CFunction<LoadSoundFunc>>()(name, info)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroySoundFunc = (CSound) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroySound: DestroySoundFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroySoundFunc")
fun ktSetDestroySoundFunc(callback: CPointer<CFunction<DestroySoundFunc>>) {
    c_destroySound = { sound ->
        memScoped {
            callback.reinterpret<CFunction<DestroySoundFunc>>()(sound)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PlaySoundFunc = (CSound) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_playSound: PlaySoundFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPlaySoundFunc")
fun ktSetPlaySoundFunc(callback: CPointer<CFunction<PlaySoundFunc>>) {
    c_playSound = { sound ->
        memScoped {
            callback.reinterpret<CFunction<PlaySoundFunc>>()(sound)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias IsSoundPlayingFunc = (CSound) -> Int

@OptIn(ExperimentalForeignApi::class)
var c_isSoundPlaying: IsSoundPlayingFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetIsSoundPlayingFunc")
fun ktSetIsSoundPlayingFunc(callback: CPointer<CFunction<IsSoundPlayingFunc>>) {
    c_isSoundPlaying = { sound ->
        memScoped {
            callback.reinterpret<CFunction<IsSoundPlayingFunc>>()(sound)
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias StopSoundFunc = (CSound) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_stopSound: StopSoundFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetStopSoundFunc")
fun ktSetStopSoundFunc(callback: CPointer<CFunction<StopSoundFunc>>) {
    c_stopSound = { sound ->
        memScoped {
            callback.reinterpret<CFunction<StopSoundFunc>>()(sound)
        }
    }
}
