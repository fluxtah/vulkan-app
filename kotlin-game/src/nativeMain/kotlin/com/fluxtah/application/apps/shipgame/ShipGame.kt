package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.apps.shipgame.ship.ForwardMovementBehavior
import com.fluxtah.application.apps.shipgame.ship.ThrustBehavior
import com.fluxtah.application.apps.shipgame.ship.YawBehavior

/*
TODO
1. Get sounds working
2. Get guns working (firing projectiles)
3. Have a reloading tile where ships fly over to reload guns
4. Introduce enemies that can be shot down
*/
class ShipGame : Application {
    override fun initialize() {
        scene("main") {
            camera("camera1") {
                position(4.0f, 5.0f, -4.0f)
                fieldOfView(90.0f)
            }
            camera("camera2") {
                position(4.0f, 5.0f, -4.0f)
            }

            light("light") {
                type(LightType.Directional)
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 50.0f, 0.0f)
                direction(0.5f, -1.0f, 0.0f)
                intensity(1.0f)
            }

            light("light2") {
                type(LightType.Point)
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 1.5f, 0.0f)
                direction(0.5f, -1.0f, 0.0f)
                intensity(1.0f)
            }

            entity("plane", "models/plane.glb") {
                position(0.0f, 0.0f, 0.0f)
                scale(10f, 10f, 10f)
            }

            entity("ship", "models/ship.glb") {
                position(0.0f, 0.0f, 0.0f)
                behaviour(ThrustBehavior(isThrusting = { isKeyPressed(Key.Up) }))
                behaviour(YawBehavior(
                    yawLeft = { isKeyPressed(Key.A) },
                    yawRight = { isKeyPressed(Key.D) }
                ))
                behaviour(ForwardMovementBehavior(isMovingForward = { isKeyPressed(Key.W) }))
            }

            sound("up-thrust", "sounds/up-thrust.wav") {
                loop(true)
            }

            var chaseCam: ChaseCamera? = null

            onSceneCreated { scene ->
                scene.setActiveCamera("camera1")
                scene.activeCamera()?.lookAt(0f, 0f, 0f)
                chaseCam = ChaseCamera(scene.activeCamera()!!, scene.entities["ship"]!!.entity)
            }

            onBeforeSceneUpdate { scene, _, deltaTime ->
                if (isKeyPressed(Key.Num1)) {
                    scene.setActiveCamera("camera1")
                }
                if (isKeyPressed(Key.Num2)) {
                    scene.setActiveCamera("camera2")
                }
            }

            onSceneUpdate { scene, _ ->
                if (scene.activeCamera() == scene.cameras["camera2"]) {
                    handleCameraInput(scene, fixedTimeStep)
                }

                chaseCam?.update(fixedTimeStep)
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