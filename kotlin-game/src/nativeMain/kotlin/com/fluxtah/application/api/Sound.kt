@file:OptIn(ExperimentalForeignApi::class)

package com.fluxtah.application.api

import com.fluxtah.application.api.interop.*
import com.fluxtah.application.api.interop.model.CreateSoundInfo
import com.fluxtah.application.api.scene.SceneDsl
import kotlinx.cinterop.ExperimentalForeignApi
import kotlinx.cinterop.cValue
import kotlinx.cinterop.cstr
import kotlinx.cinterop.memScoped

class Sound(val handle: CSound) {
    fun play() {
        c_playSound!!.invoke(handle)
    }

    fun playIfNotPlaying() {
        if (!isPlaying()) {
            play()
        }
    }

    fun isPlaying(): Boolean {
        return c_isSoundPlaying!!.invoke(handle) == 1
    }

    fun stop() {
        c_stopSound!!.invoke(handle)
    }

    fun stopIfPlaying() {
        if (isPlaying()) {
            stop()
        }
    }

    fun setPitch(pitch: Float) {
        c_setSoundPitch!!.invoke(handle, pitch)
    }
}

@SceneDsl
class SoundBuilder(private val soundPath: String) {
    private var loop: Boolean = false

    fun loop(enabled: Boolean) {
        loop = enabled
    }

    fun build(): Sound {
        val info = cValue<CreateSoundInfo> {
            loop = if (this@SoundBuilder.loop) 1 else 0
        }
        val cSound = memScoped { c_loadSound!!.invoke(soundPath.cstr.ptr, info.ptr) }
        return Sound(cSound)
    }
}