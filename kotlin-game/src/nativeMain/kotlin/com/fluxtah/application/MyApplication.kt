package com.fluxtah.application

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key

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
                onSceneUpdate { _, entity, _, deltaTime ->
                    entity.rotate(0.0f, 20.5f * deltaTime, 20.5f * deltaTime)
                }
            }

            entity("sphere2", "models/sphere.glb") {
                position(2.0f, 0.0f, 0.0f)
                onSceneUpdate { _, entity, _, deltaTime ->
                    entity.rotate(0.0f, 20.5f * deltaTime, 20.5f * deltaTime)
                }
            }

            entity("sphere3", "models/sphere.glb") {
                position(2.0f, 0.0f, 2.0f)
                onSceneUpdate { _, entity, _, deltaTime ->
                    entity.rotate(0.0f, 20.5f * deltaTime, 20.5f * deltaTime)
                }
            }


            onSceneCreated { scene ->
                scene.setActiveCamera("camera")
            }

            onSceneUpdate { scene, _, deltaTime ->
                handleMoveCamera(scene, deltaTime)
            }
        }

        setActiveScene("main")
    }
}

private fun handleMoveCamera(scene: Scene, deltaTime: Float) {
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
