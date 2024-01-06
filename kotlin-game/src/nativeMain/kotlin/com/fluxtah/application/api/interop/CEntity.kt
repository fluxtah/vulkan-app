package com.fluxtah.application.api.interop

import com.fluxtah.application.api.activeScene
import com.fluxtah.application.api.interop.model.CreateEntityInfo
import com.fluxtah.application.api.interop.model.EntityArray
import com.fluxtah.application.api.interop.model.LightArray
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class)
typealias CEntity = CPointer<CPointed>

@OptIn(ExperimentalForeignApi::class)
typealias CCreateEntityInfo = CPointer<CreateEntityInfo>

@OptIn(ExperimentalForeignApi::class)
typealias CreateEntityFunc = (CVulkanContext, CPointer<ByteVar>, CCreateEntityInfo) -> CEntity

@OptIn(ExperimentalForeignApi::class)
var c_createEntity: CreateEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetCreateEntityFunc")
fun ktSetCreateEntityFunc(callback: CPointer<CFunction<CreateEntityFunc>>) {
    c_createEntity = { context, name, info ->
        memScoped {
            callback.reinterpret<CFunction<CreateEntityFunc>>()(
                context,
                name,
                info
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias DestroyEntityFunc = (CVulkanContext, CEntity) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_destroyEntity: DestroyEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetDestroyEntityFunc")
fun ktSetDestroyEntityFunc(callback: CPointer<CFunction<(CVulkanContext, CEntity) -> Unit>>) {
    c_destroyEntity = { device, renderObject ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CEntity) -> Unit>>()(device, renderObject)
        }
    }
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetEntities")
fun ktGetEntities(): CPointer<EntityArray> {
    if (activeScene == null) {
        throw RuntimeException("No active scene")
    }
    val entities = activeScene!!.scene.entities.values.map { it.entity.handle } // Assuming handle is COpaquePointer
    val entityPointerArray = nativeHeap.allocArray<COpaquePointerVar>(entities.size)

    entities.forEachIndexed { index, light ->
        entityPointerArray[index] = light
    }

    val entityArray = nativeHeap.alloc<EntityArray>()
    entityArray.entities = entityPointerArray
    entityArray.size = entities.size

    return entityArray.ptr
}


@OptIn(ExperimentalForeignApi::class)
typealias RotateEntityFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_rotateEntity: RotateEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetRotateEntityFunc")
fun ktSetRotateEntityFunc(callback: CPointer<CFunction<(CEntity, Float, Float, Float) -> Unit>>) {
    c_rotateEntity = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<(CEntity, Float, Float, Float) -> Unit>>()(
                entity, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias TranslateEntityFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_translateEntity: TranslateEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetTranslateEntityFunc")
fun ktSetTranslateEntityFunc(callback: CPointer<CFunction<(CEntity, Float, Float, Float) -> Unit>>) {
    c_translateEntity = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<(CEntity, Float, Float, Float) -> Unit>>()(
                entity, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias PositionEntityFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_positionEntity: PositionEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPositionEntityFunc")
fun ktSetPositionEntityFunc(callback: CPointer<CFunction<(CEntity, Float, Float, Float) -> Unit>>) {
    c_positionEntity = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<(CEntity, Float, Float, Float) -> Unit>>()(
                entity, x, y, z
            )
        }
    }
}