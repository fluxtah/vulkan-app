cmake_minimum_required(VERSION 3.10)
project(Game C)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenAL REQUIRED)

add_executable(Game
        src/main.c

        include/window.h
        src/window.c

        include/modeldata.h
        src/modeldata.c

        include/renderresources.h
        src/renderresources.c

        include/ubo.h
        include/ubo_update.h
        src/ubo_update.c

        include/util_strings.h
        src/util_strings.c

        include/vulkan/instance.h
        src/vulkan/instance.c

        include/vulkan/physicaldevice.h
        src/vulkan/physicaldevice.c

        include/vulkan/setup.h
        src/vulkan/setup.c

        include/vulkan/swapchain.h
        src/vulkan/swapchain.c

        include/vulkan/shaders.h
        src/vulkan/shaders.c

        include/vulkan/pipeline.h
        src/vulkan/pipeline.c

        include/vulkan/renderpass.h
        src/vulkan/renderpass.c

        include/vulkan/framebuffer.h
        src/vulkan/framebuffer.c

        include/vulkan/commandpool.h
        src/vulkan/commandpool.c

        include/vulkan/commandbuffer.h
        src/vulkan/commandbuffer.c

        include/vulkan/buffer.h
        src/vulkan/buffer.c

        include/vulkan/descriptor.h
        src/vulkan/descriptor.c

        include/vulkan/depth.h
        src/vulkan/depth.c

        include/vulkan/memory.h
        src/vulkan/memory.c

        include/vulkan/image.h
        src/vulkan/image.c

        include/vulkan/sampler.h
        src/vulkan/sampler.c

        include/context.h

        include/camera.h
        src/camera.c

        include/vulkan/render.h
        src/vulkan/render.c

        libs/stb_image.h
        src/light.c

        include/light.h

        libs/dr_wav.h
        src/sound.c
        include/sound.h

        include/kotlin.h
        src/kotlin.c

        include/pipelineconfig.h
        src/pipelineconfig.c

        include/imagememory.h
        src/imagememory.c
        libs/uthash.h

        include/debug.h
        src/debug.c
)

include_directories(kotlin-game/build/bin/native/releaseShared)
include_directories(kotlin-game/cinterop)
target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Game PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(Game PRIVATE ${OPENAL_INCLUDE_DIRS})

target_link_libraries(Game OpenAL::OpenAL)

add_library(kotlin_library SHARED IMPORTED)
set_target_properties(kotlin_library PROPERTIES
        IMPORTED_LOCATION "kotlin-game/build/bin/native/releaseShared/libkotlin_game.dylib"
)
add_library(cglm SHARED IMPORTED)
set_target_properties(cglm PROPERTIES
        IMPORTED_LOCATION "/opt/homebrew/opt/cglm/lib/libcglm.dylib"
)

target_link_libraries(Game ${Vulkan_LIBRARIES} glfw cglm kotlin_library)