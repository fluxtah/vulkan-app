package com.fluxtah.application

import com.fluxtah.application.api.Application
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.isKeyPressed
import com.fluxtah.application.api.scene
import com.fluxtah.application.api.setActiveScene

class MyApplication : Application {
    override fun initialize() {

        scene("main") {
            camera("camera") {
                position(0.0f, 0.0f, 2.0f)
            }

//            light("light") {
//                color(1.0f, 1.0f, 1.0f, 1.0f)
//                position(0.0f, 1.0f, 1.7f)
//            }
//
//            entity("plane", "models/plane.glb") {
//                position(0.0f, 0.0f, 0.0f)
//            }
//            entity("sphere", "models/sphere.glb") {
//                position(0.0f, 0.0f, 0.0f)
//                onEachFrame { deltaTime ->
//                    rotate(0.0f, 0.1f * deltaTime, 0.02f * deltaTime)
//                }
//            }

            onSceneCreated { scene ->
                scene.setActiveCamera("camera")
            }
        }

        setActiveScene("main")
    }

    override fun update(time: Float, deltaTime: Float) {
        handleMoveCamera(deltaTime)
    }

    private fun handleMoveCamera(deltaTime: Float) {
        val camera = scene().activeCamera() ?: return
        val baseSpeed = 3.0f
        val cameraSpeed = baseSpeed * deltaTime
        val rotationSpeed = 40.0f
        val cameraRotationSpeed = rotationSpeed * deltaTime

        if (isKeyPressed(Key.W)) {
            camera.moveForward(cameraSpeed)
        }
        if (isKeyPressed(Key.S)) {
            camera.moveBackward(cameraSpeed)
        }
        if (isKeyPressed(Key.A)) {
            camera.moveLeft(cameraSpeed)
        }
        if (isKeyPressed(Key.D)) {
            camera.moveRight(cameraSpeed)
        }

        if (isKeyPressed(Key.Up)) {
            camera.pitchUp(cameraRotationSpeed)
        }
        if (isKeyPressed(Key.Down)) {
            camera.pitchDown(cameraRotationSpeed)
        }
        if (isKeyPressed(Key.Left)) {
            camera.yawLeft(cameraRotationSpeed)
        }
        if (isKeyPressed(Key.Right)) {
            camera.yawRight(cameraRotationSpeed)
        }

        camera.applyChanges()
    }
}
