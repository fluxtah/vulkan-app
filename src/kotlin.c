#include "include/kotlin.h"
#include "include/vulkan/setup.h"

void setActiveCamera(ApplicationContext *context, Camera *camera) {
    context->activeCamera = camera;

    // Default to extent
    if (camera->aspectRatio == 0.0f) {
        camera->aspectRatio = (float) context->vulkanSwapchainContext->swapChainExtent.width /
                              (float) context->vulkanSwapchainContext->swapChainExtent.height;
        applyCameraChanges(camera);
    }
}

int isKeyPressed(int key) {
    if (keys[key]) {
        return 1;
    }

    return -1;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void bindKotlinApi() {
    // Application
    ktSetEnableDebugBoundingVolumesFunc(enableDebugBoundingVolumes);
    // Input
    ktSetIsKeyPressedFunc(isKeyPressed);

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
    ktSetCreateEntityFunc(createEntity);
    ktSetDestroyEntityFunc(destroyEntity);
    ktSetPositionEntityFunc(setEntityPosition);
    ktSetEntityRotationFunc(setEntityRotation);
    ktSetEntityScaleFunc(setEntityScale);

    // Sound
    ktSetLoadSoundFunc(loadSound);
    ktSetDestroySoundFunc(destroySound);
    ktSetPlaySoundFunc(playSound);
    ktSetIsSoundPlayingFunc(isSoundPlaying);
    ktSetStopSoundFunc(stopSound);
    ktSetSoundPitchFunc(setSoundPitch);
}