package com.fluxtah.application.apps.shipgame

import kotlin.math.floor

val tileSize: Int = 10
class InfiniteMap(val seed: Int) {
    private val noise = PerlinNoise(seed)  // Assuming PerlinNoise is a properly implemented class

    private fun getTile(gridX: Int, gridZ: Int): Tile {
        val noiseValue = noise.generate(gridX.toDouble() * 1.3333, gridZ.toDouble() * 1.3333)
        return Tile(gridX, gridZ, determineTileType(noiseValue))
    }

    private fun determineTileType(noiseValue: Double): TileType = if(noiseValue < 0.3) {
        TileType.LEVEL1
    } else if(noiseValue < 0.5) {
        TileType.LEVEL2
    } else if (noiseValue < 0.7) {
        TileType.LEVEL3
    } else {
        TileType.LEVEL4
    }

    fun getTileByWorldPosition(posX: Float, posZ: Float, range: Int): List<Tile> {
        val playerTileX = floor(posX / tileSize).toInt()
        val playerTileZ = floor(posZ / tileSize).toInt()

        return generateSurroundingTiles(playerTileX, playerTileZ, range)
    }

    private fun generateSurroundingTiles(centerX: Int, centerZ: Int, range: Int): List<Tile> {
        val tiles = mutableListOf<Tile>()
        for (x in (centerX - range)..(centerX + range)) {
            for (z in (centerZ - range)..(centerZ + range)) {
                tiles.add(getTile(x, z))
            }
        }
        return tiles
    }
}

enum class TileType {
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
}

data class Tile(val gridX: Int, val gridZ: Int, val type: TileType) {
    // Calculate the world position of the tile based on its center
    val worldX = gridX * tileSize + tileSize / 2
    val worldZ = gridZ * tileSize + tileSize / 2

    // ... Additional methods and properties
}

