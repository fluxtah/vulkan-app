#include "include/kotlin.h"

void setActiveCamera(ApplicationContext *context, Camera *camera) {
    context->activeCamera = camera;

    // Default to extent
    if (camera->aspectRatio == 0.0f) {
        camera->aspectRatio = (float) context->swapChainExtent.width / (float) context->swapChainExtent.height;
        applyCameraChanges(camera);
    }
}

void bindKotlinApi() {
    // Camera
    ktSetCreateCameraFunc(createCamera);
    ktSetDestroyCameraFunc(destroyCamera);
    ktSetMoveCameraForwardFunc(moveCameraForward);
    ktSetMoveCameraBackwardFunc(moveCameraBackward);
    ktSetMoveCameraLeftFunc(moveCameraLeft);
    ktSetMoveCameraRightFunc(moveCameraRight);
    ktSetPitchCameraFunc(pitchCamera);
    ktSetYawCameraFunc(yawCamera);
    ktSetPositionCameraFunc(positionCamera);
    ktSetApplyCameraChangesFunc(applyCameraChanges);
    ktSetActiveCameraFunc(setActiveCamera);
    ktSetCameraLookAtFunc(setCameraLookAt);

    // Light
    ktSetCreateLightFunc(createLight);
    ktSetDestroyLightFunc(destroyLight);

    // Entity
    ktSetCreateEntityFunc(createRenderObjectFromFile);
    ktSetDestroyEntityFunc(destroyRenderObject);
    ktSetRotateEntityFunc(rotateRenderObject);
    ktSetPositionEntityFunc(positionRenderObject);
    ktSetTranslateEntityFunc(translateRenderObject);

    // Sound
    ktSetLoadSoundFunc(loadSound);
    ktSetDestroySoundFunc(destroySound);
    ktSetPlaySoundFunc(playSound);
    ktSetIsSoundPlayingFunc(isSoundPlaying);
    ktSetStopSoundFunc(stopSound);
    ktSetSoundPitchFunc(setSoundPitch);
}