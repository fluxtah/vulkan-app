package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.apps.shipgame.ship.ThrustBehavior

class ShipGame : Application {
    override fun initialize() {
        scene("main") {
            camera("camera1") {
                position(4.0f, 5.0f, -4.0f)
            }
            camera("camera2") {
                position(4.0f, 5.0f, -4.0f)
            }

            light("light") {
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 1.0f, 1.7f)
            }

            entity("plane", "models/plane.glb") {
                position(0.0f, 0.0f, 0.0f)
            }

            entity("ship", "models/ship.glb") {
                position(0.0f, 0.0f, 0.0f)
                behaviour(ThrustBehavior(isThrusting = { isKeyPressed(Key.Equal) }))
            }

            var chaseCam: ChaseCamera? = null

            onSceneCreated { scene ->
                scene.setActiveCamera("camera1")
                scene.activeCamera()?.lookAt(0f, 0f, 0f)
                chaseCam = ChaseCamera(scene.activeCamera()!!, scene.entities["ship"]!!.entity)
            }

            onSceneUpdate { scene, _, deltaTime ->
                if (isKeyPressed(Key.Num1)) {
                    scene.setActiveCamera("camera1")
                }
                if (isKeyPressed(Key.Num2)) {
                    scene.setActiveCamera("camera2")
                }

                handleCameraInput(scene, deltaTime)
                if (scene.activeCamera() == scene.cameras["camera2"]) {
                    handleCameraInput(scene, deltaTime)
                }

                chaseCam?.update(deltaTime)
            }

        }
        setActiveScene("main")
    }

    private fun handleCameraInput(scene: Scene, deltaTime: Float) {
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
}