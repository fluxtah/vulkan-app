package com.fluxtah.application

import com.fluxtah.application.api.*

class MyApplication : Application {
    override fun initialize() {
        scene("main") {
            camera("camera") {
                position(0.0f, 0.0f, 2.0f)
            }

            light("light") {
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 1.0f, 1.7f)
            }

            entity("plane", "models/plane.glb") {
                position(0.0f, 0.0f, 0.0f)
            }
            entity("sphere", "models/sphere.glb") {
                position(0.0f, 0.0f, 0.0f)
                onEachFrame { deltaTime ->
                    rotate(0.0f, 0.1f * deltaTime, 0.02f * deltaTime)
                }
            }
        }

        setActiveScene("main")
        setActiveCamera("camera")
    }
}
