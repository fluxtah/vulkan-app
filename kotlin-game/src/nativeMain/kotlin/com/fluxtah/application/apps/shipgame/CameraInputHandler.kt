package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.scene.Scene
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.isKeyPressed

fun handleCameraInput(scene: Scene, deltaTime: Float) {
    val camera = scene.activeCamera() ?: return
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