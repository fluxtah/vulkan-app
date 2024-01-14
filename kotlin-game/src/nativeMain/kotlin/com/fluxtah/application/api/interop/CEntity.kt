package com.fluxtah.application.api.interop

import com.fluxtah.application.api.scene.SceneImpl
import com.fluxtah.application.api.scene.activeSceneInfo
import com.fluxtah.application.api.interop.model.CreateEntityInfo
import com.fluxtah.application.api.interop.model.EntityArray
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
    c_destroyEntity = { device, renderResources ->
        memScoped {
            callback.reinterpret<CFunction<(CVulkanContext, CEntity) -> Unit>>()(device, renderResources)
        }
    }
}

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktGetEntities")
fun ktGetEntities(): CPointer<EntityArray> {
    val scene = activeSceneInfo.scene as SceneImpl
    val entities = scene.entities.values.filter { it.entity.visible }.map { it.entity.handle } +
            scene.entityPools.flatMap { it.value.entitiesInUse.filter { it.entity.visible }.map { it.entity.handle } }

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
typealias EntityPositionFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEntityPosition: EntityPositionFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetPositionEntityFunc")
fun ktSetPositionEntityFunc(callback: CPointer<CFunction<(CEntity, Float, Float, Float) -> Unit>>) {
    c_setEntityPosition = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<(CEntity, Float, Float, Float) -> Unit>>()(
                entity, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias EntityRotationFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEntityRotation: EntityRotationFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetEntityRotationFunc")
fun ktSetEntityRotationFunc(callback: CPointer<CFunction<EntityRotationFunc>>) {
    c_setEntityRotation = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<EntityRotationFunc>>()(
                entity, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias EntityScaleFunc = (CEntity, Float, Float, Float) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_setEntityScale: EntityScaleFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetEntityScaleFunc")
fun ktSetEntityScaleFunc(callback: CPointer<CFunction<EntityScaleFunc>>) {
    c_setEntityScale = { entity, x, y, z ->
        memScoped {
            callback.reinterpret<CFunction<EntityScaleFunc>>()(
                entity, x, y, z
            )
        }
    }
}

@OptIn(ExperimentalForeignApi::class)
typealias AttachKotlinEntityFunc = (CEntity, COpaquePointer) -> Unit

@OptIn(ExperimentalForeignApi::class)
var c_attachKotlinEntity: AttachKotlinEntityFunc? = null

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktSetAttachKotlinEntityFunc")
fun ktSetAttachKotlinEntityFunc(callback: CPointer<CFunction<AttachKotlinEntityFunc>>) {
    c_attachKotlinEntity = { entity, kotlinEntityPtr ->
        memScoped {
            callback.reinterpret<CFunction<AttachKotlinEntityFunc>>()(
                entity, kotlinEntityPtr
            )
        }
    }
}
