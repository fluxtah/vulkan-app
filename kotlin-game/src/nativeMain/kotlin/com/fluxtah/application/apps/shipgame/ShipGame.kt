package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.apps.shipgame.behaviors.*
import kotlin.random.Random

/*
TODO:
 ✓ Get asteroids working
    * Get asteroids to spawn randomly without overlapping
 * Get projectiles to destroy asteroids (collision detection)
    * Get OBB and AABB collision detection working
 * Get ship to explode when it hits an asteroid
 * High Score
    * Get text rendering working, maybe vector text?
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
                // useOrientedBoundingBox()
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
                //useOrientedBoundingBox()
                initialSize(5)
                behaviour {
                    PlasmaBoltBehaviour()
                }
                onCollision { scene, entity, entities ->
                    println("${entity.id} collided with ${entities.size} entities")
                    entities.forEach { otherEntity ->
                        if (otherEntity.id == Id.ENT_ASTEROID) {
                            scene.entityToPool(entity)
                            scene.entityToPool(otherEntity)
                            scene.soundById(Id.SOUND_ASTEROID_EXPLODE)?.play()

                            entity.visible = false
                            otherEntity.visible = false
                        }
                    }
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
            sound(Id.SOUND_ASTEROID_EXPLODE, "sounds/asteroid-explode.wav")

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
                if (isKeyPressed(Key.F1)) {
                    enableDebugBoundingVolumes(true)
                }
                if (isKeyPressed(Key.F2)) {
                    enableDebugBoundingVolumes(false)
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
}
