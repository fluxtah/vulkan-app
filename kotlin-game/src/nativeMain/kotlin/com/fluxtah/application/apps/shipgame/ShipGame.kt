package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key

class ShipGame : Application {
    override fun initialize() {
        scene("main") {
            camera("camera1") {
                position(0.0f, 5.0f, 4.0f)
            }

            light("light") {
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 1.0f, 1.7f)
            }

            entity("plane", "models/plane.glb") {
                position(0.0f, 0.0f, 0.0f)
            }

            spaceship()

            onSceneCreated { scene ->
                scene.setActiveCamera("camera1")
                scene.activeCamera()?.pitchDown(45.0f)
                scene.activeCamera()?.yawLeft(180.0f)
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