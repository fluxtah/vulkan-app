package com.fluxtah.application.api.entity

import com.fluxtah.application.api.scene.EntityInfo
import com.fluxtah.application.api.scene.activeSceneInfo
import com.fluxtah.application.apps.shipgame.Id
import kotlinx.cinterop.*
import kotlin.experimental.ExperimentalNativeApi

@OptIn(ExperimentalForeignApi::class, ExperimentalNativeApi::class)
@CName("ktCollisionCallback")
fun ktCollisionCallback(entityInfoPtr: COpaquePointer, entityInfosArrayPtr: CPointer<COpaquePointerVar>, size: Int) {
    val entityInfo = entityInfoPtr.asStableRef<EntityInfo>().get()
    val entityList = mutableListOf<Entity>()

    if(!entityInfo.entity.inUse) return

    for (i in 0 until size) {
        val ptr = entityInfosArrayPtr[i]
        val info = ptr!!.asStableRef<EntityInfo>().get()
        if(info.entity.inUse) {
            entityList.add(info.entity)
        }
    }

    if(entityList.isEmpty()) return

    entityInfo.onCollision?.invoke(activeSceneInfo.scene, entityInfo.entity, entityList)
}