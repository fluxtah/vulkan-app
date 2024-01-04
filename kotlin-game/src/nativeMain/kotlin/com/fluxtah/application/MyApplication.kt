package com.fluxtah.application

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key

class MyApplication : Application {
    override fun initialize() {
        scene("main") {
            camera("camera1") {
                position(0.0f, 0.0f, 2.0f)
            }
            camera("camera2") {
                position(0.0f, 0.0f, 4.0f)
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
                onSceneUpdate { _, entity, _, deltaTime ->
                    entity.rotate(0.0f, 20.5f * deltaTime, 20.5f * deltaTime)
                }
            }

            onSceneCreated { scene ->
                scene.setActiveCamera("camera1")
            }

            onSceneUpdate { scene, _, deltaTime ->
                handleCameraInput(scene, deltaTime)
            }
        }

        setActiveScene("main")
    }
}

private fun handleCameraInput(scene: Scene, deltaTime: Float) {
    val camera = scene.activeCamera() ?: return
    val baseSpeed = 3.0f
    val cameraSpeed = baseSpeed * deltaTime
    val rotationSpeed = 40.0f
    val cameraRotationSpeed = rotationSpeed * deltaTime

    if (isKeyPressed(Key.Num1)) {
        scene.setActiveCamera("camera1")
    }
    if (isKeyPressed(Key.Num2)) {
        scene.setActiveCamera("camera2")
    }

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
        camera.pitch(cameraRotationSpeed)
    }
    if (isKeyPressed(Key.Down)) {
        camera.pitch(-cameraRotationSpeed)
    }
    if (isKeyPressed(Key.Left)) {
        camera.yaw(cameraRotationSpeed)
    }
    if (isKeyPressed(Key.Right)) {
        camera.yaw(-cameraRotationSpeed)
    }

    camera.applyChanges()
}
