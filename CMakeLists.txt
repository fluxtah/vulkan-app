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

        include/imagememory.h
        src/imagememory.c
        libs/uthash.h

        include/debug.h
        src/debug.c

        include/push_constants.h
        include/debug_cube.h
        include/vulkan/buffer_memory.h
        include/vertex.h

        include/entity.h
        src/entity.c

        include/obb.h

        include/aabb.h
        src/aabb.c

        include/pipelines/pipeline_config_shared.h
        src/pipelines/pipeline_config_shared.c

        include/pipelines/basic/basic_pipeline_config.h
        src/pipelines/basic/basic_pipeline_config.c

        include/pipelines/debug/debug_pipeline_config.h
        src/pipelines/debug/debug_pipeline_config.c

        include/pipelines/basic/basic_pipeline_layout.h
        src/pipelines/basic/basic_pipeline_layout.c

        include/pipelines/debug/debug_pipeline_layout.h
        src/pipelines/debug/debug_pipeline_layout.c

        include/pipelines/basic/basic_pipeline.h
        src/pipelines/basic/basic_pipeline.c

        include/pipelines/debug/debug_pipeline.h
        src/pipelines/debug/debug_pipeline.c

        include/pipelines/viewport_util.h
        src/pipelines/viewport_util.c

        include/pipelines/pipeline_shader_util.h
        src/pipelines/pipeline_shader_util.c

        include/pipelines/basic/basic_pipeline_renderpass.h
        src/pipelines/basic/basic_pipeline_renderpass.c

        include/pipelines/debug/debug_pipeline_renderpass.h
        src/pipelines/debug/debug_pipeline_renderpass.c
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