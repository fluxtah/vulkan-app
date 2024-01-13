package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.apps.shipgame.behaviors.*
import kotlin.random.Random

/*
TODO
✓ Get sounds working
✓ Get guns working (firing projectiles)
* Check over entity pool code and make sure its working including support code like behaviors
* Get asteroids working
   * Get asteroids to spawn randomly without overlapping
* Get projectiles to destroy asteroids (collision detection)
* Get ship to explode when it hits an asteroid
* High Score
*/
class ShipGame : Application {
    override fun initialize() {
        scene(Id.SCENE_MAIN) {
            camera(Id.CAMERA1) {
                position(4.0f, 6.0f, -4.0f)
                fieldOfView(60.0f)
            }
            camera(Id.CAMERA2) {
                position(4.0f, 5.0f, -4.0f)
            }

            light(Id.LIGHT1) {
                type(LightType.Directional)
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 50.0f, 0.0f)
                direction(-1.0f, -1.0f, 0.0f)
                intensity(1.4f)
            }

            light(Id.LIGHT2) {
                type(LightType.Point)
                color(1.0f, 1.0f, 1.0f, 1.0f)
                position(0.0f, 1.5f, 0.0f)
                direction(0.5f, -1.0f, 0.0f)
                intensity(1.0f)
            }

            entity(Id.ENT_PLANE, "models/plane.glb") {
                position(0.0f, -0.1f, 0.0f)
                scale(10f, 10f, 10f)
            }

            entity(Id.ENT_SHIP, "models/ship.glb") {
                position(0.0f, 0.0f, 0.0f)
                behaviour { FirePlasmaCannonBehaviour(fireButtonPressed = { isKeyPressed(Key.Space) }) }
                behaviour { ThrustBehavior(isThrusting = { isKeyPressed(Key.Up) }) }
                behaviour {
                    YawBehavior(
                        yawLeft = { isKeyPressed(Key.A) },
                        yawRight = { isKeyPressed(Key.D) }
                    )
                }
                behaviour {
                    ForwardMovementBehavior(
                        isMovingForward = { isKeyPressed(Key.W) },
                        isReversing = { isKeyPressed(Key.S) })
                }
            }

            entityPool(Id.ENT_PLASMA_BOLT, "models/plasma-bolt.glb") {
                initialSize(5)
                behaviour {
                    PlasmaBoltBehaviour()
                }
            }

            entityPool(Id.ENT_ASTEROID, "models/asteroid.glb") {
                initialSize(50)
                startActive()
                behaviour {
                    AsteroidBehavior(
                        speedX = Random.nextFloat() * 50,
                        speedY = Random.nextFloat() * 50,
                        speedZ = Random.nextFloat() * 50
                    )
                }
            }

            sound(Id.SOUND_UP_THRUST, "sounds/up-thrust.wav") {
                loop(true)
            }
            sound(Id.SOUND_ENGINE, "sounds/engine.wav") {
                loop(true)
            }
            sound(Id.SOUND_SONIC_BOOM, "sounds/sonic-boom.wav")
            sound(Id.SOUND_PLASMA_BOLT, "sounds/plasma-bolt.wav")

            var chaseCam: ChaseCamera? = null

            onSceneCreated { scene ->
                scene.setActiveCamera(Id.CAMERA1)
                scene.activeCamera()?.lookAt(0f, 0f, 0f)
                chaseCam = ChaseCamera(scene.activeCamera()!!, scene.entityById(Id.ENT_SHIP)!!)
            }

            onBeforeSceneUpdate { scene, _, _ ->
                if (isKeyPressed(Key.Num1)) {
                    scene.setActiveCamera(Id.CAMERA1)
                }
                if (isKeyPressed(Key.Num2)) {
                    scene.setActiveCamera(Id.CAMERA2)
                }
            }

            onSceneUpdate { scene, _ ->
                if (scene.activeCamera() == scene.cameraById(Id.CAMERA2)) {
                    handleCameraInput(scene, fixedTimeStep)
                }

                chaseCam?.update(fixedTimeStep)
            }
        }

        setActiveScene(Id.SCENE_MAIN)
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

object Id {
    const val SCENE_MAIN = "main"

    const val CAMERA1 = "camera1"
    const val CAMERA2 = "camera2"

    const val LIGHT1 = "light1"
    const val LIGHT2 = "light2"

    const val ENT_PLANE = "plane"
    const val ENT_SHIP = "ship"
    const val ENT_PLASMA_BOLT = "plasma-bolt"
    const val ENT_ASTEROID = "asteroid"

    const val SOUND_UP_THRUST = "up-thrust"
    const val SOUND_ENGINE = "engine"
    const val SOUND_SONIC_BOOM = "sonic-boom"
    const val SOUND_PLASMA_BOLT = "plasma-bolt"
}