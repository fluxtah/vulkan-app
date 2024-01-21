package com.fluxtah.application.apps.shipgame

import com.fluxtah.application.api.*
import com.fluxtah.application.api.input.Key
import com.fluxtah.application.api.scene.scene
import com.fluxtah.application.api.scene.setActiveScene
import com.fluxtah.application.apps.shipgame.behaviors.*
import kotlin.random.Random

/*
TODO:
 ✓ Get asteroids working
    * Get asteroids to spawn randomly without overlapping
    * Ship to asteroid collision, both ship and asteroid should explode
    * When ship explodes need a sequence/animation/action camera cutscene
 ✓ Get projectiles to destroy asteroids (collision detection)
    * Get OBB and AABB collision detection working
 ✓ Particle emitters
 * Instanced rendering for asteroids
 * Tile based map like a racing game
    * Dynamic layout of tiles based on map data, as player moves forward and back
       * octree?
 * Scene switching
    * Main menu
    * Game over
 * Get ship to explode when it hits an asteroid
 * High Score
    * Get text rendering working, maybe vector text?
 * Improve logging
 * Targeting system
    * Cycle through targets (asteroids)
    * Targeting reticle to show which asteroid is targeted
    * Missiles that hone in on the target
 * UI system
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
                behaviour {
                    LateralMovementBehavior(
                        isMovingLeft = { isKeyPressed(Key.Left) },
                        isMovingRight = { isKeyPressed(Key.Right) }
                    )
                }
            }

            entityPool(Id.ENT_PLASMA_BOLT, "models/plasma-bolt.glb") {
                //useOrientedBoundingBox()
                initialSize(5)
                behaviour {
                    PlasmaBoltBehaviour()
                }
                onCollision { scene, plasmaBoltEntity, entities ->
                    for (otherEntity in entities) {
                        if (otherEntity.id == Id.ENT_ASTEROID) {
                            val asteroidDieBehavior = otherEntity.getBehaviorByType<AsteroidDieBehavior>()
                            scene.entityToPool(plasmaBoltEntity)
                            asteroidDieBehavior.die()
                            plasmaBoltEntity.visible = false
                            return@onCollision
                        }
                    }
                }
            }

            entityPool(Id.ENT_ASTEROID, "models/asteroid.glb") {
                initialSize(50)
                startActive()
                behaviour { AsteroidDieBehavior() }
                behaviour {
                    AsteroidMovementBehavior(
                        speedX = Random.nextFloat() * 50,
                        speedY = Random.nextFloat() * 50,
                        speedZ = Random.nextFloat() * 50
                    )
                }
            }

            emitterPool(Id.EMITTER_EXPLOSION, "models/quad-explosion.glb") {
                initialSize(3)
                maxParticles(20)
                behaviour { ExplosionEmitterBehavior() }
            }

            sound(Id.SOUND_UP_THRUST, "sounds/up-thrust.wav") {
                loop(true)
            }
            sound(Id.SOUND_ENGINE, "sounds/engine.wav") {
                loop(true)
            }
            sound(Id.SOUND_LATERAL_THRUST, "sounds/lateral-thrust.wav") {
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

                chaseCam?.update()
            }
        }

        setActiveScene(Id.SCENE_MAIN)
    }
}
