# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ian.warwick/Documents/sdev/game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ian.warwick/Documents/sdev/game

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/ian.warwick/Documents/sdev/game/CMakeFiles /Users/ian.warwick/Documents/sdev/game//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/ian.warwick/Documents/sdev/game/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Game

# Build rule for target.
Game: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Game
.PHONY : Game

# fast build rule for target.
Game/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/build
.PHONY : Game/fast

src/camera.o: src/camera.c.o
.PHONY : src/camera.o

# target to build an object file
src/camera.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/camera.c.o
.PHONY : src/camera.c.o

src/camera.i: src/camera.c.i
.PHONY : src/camera.i

# target to preprocess a source file
src/camera.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/camera.c.i
.PHONY : src/camera.c.i

src/camera.s: src/camera.c.s
.PHONY : src/camera.s

# target to generate assembly for a file
src/camera.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/camera.c.s
.PHONY : src/camera.c.s

src/imagememory.o: src/imagememory.c.o
.PHONY : src/imagememory.o

# target to build an object file
src/imagememory.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/imagememory.c.o
.PHONY : src/imagememory.c.o

src/imagememory.i: src/imagememory.c.i
.PHONY : src/imagememory.i

# target to preprocess a source file
src/imagememory.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/imagememory.c.i
.PHONY : src/imagememory.c.i

src/imagememory.s: src/imagememory.c.s
.PHONY : src/imagememory.s

# target to generate assembly for a file
src/imagememory.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/imagememory.c.s
.PHONY : src/imagememory.c.s

src/kotlin.o: src/kotlin.c.o
.PHONY : src/kotlin.o

# target to build an object file
src/kotlin.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/kotlin.c.o
.PHONY : src/kotlin.c.o

src/kotlin.i: src/kotlin.c.i
.PHONY : src/kotlin.i

# target to preprocess a source file
src/kotlin.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/kotlin.c.i
.PHONY : src/kotlin.c.i

src/kotlin.s: src/kotlin.c.s
.PHONY : src/kotlin.s

# target to generate assembly for a file
src/kotlin.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/kotlin.c.s
.PHONY : src/kotlin.c.s

src/light.o: src/light.c.o
.PHONY : src/light.o

# target to build an object file
src/light.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/light.c.o
.PHONY : src/light.c.o

src/light.i: src/light.c.i
.PHONY : src/light.i

# target to preprocess a source file
src/light.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/light.c.i
.PHONY : src/light.c.i

src/light.s: src/light.c.s
.PHONY : src/light.s

# target to generate assembly for a file
src/light.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/light.c.s
.PHONY : src/light.c.s

src/main.o: src/main.c.o
.PHONY : src/main.o

# target to build an object file
src/main.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.c.o
.PHONY : src/main.c.o

src/main.i: src/main.c.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.c.i
.PHONY : src/main.c.i

src/main.s: src/main.c.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/main.c.s
.PHONY : src/main.c.s

src/modeldata.o: src/modeldata.c.o
.PHONY : src/modeldata.o

# target to build an object file
src/modeldata.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/modeldata.c.o
.PHONY : src/modeldata.c.o

src/modeldata.i: src/modeldata.c.i
.PHONY : src/modeldata.i

# target to preprocess a source file
src/modeldata.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/modeldata.c.i
.PHONY : src/modeldata.c.i

src/modeldata.s: src/modeldata.c.s
.PHONY : src/modeldata.s

# target to generate assembly for a file
src/modeldata.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/modeldata.c.s
.PHONY : src/modeldata.c.s

src/pipelineconfig.o: src/pipelineconfig.c.o
.PHONY : src/pipelineconfig.o

# target to build an object file
src/pipelineconfig.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/pipelineconfig.c.o
.PHONY : src/pipelineconfig.c.o

src/pipelineconfig.i: src/pipelineconfig.c.i
.PHONY : src/pipelineconfig.i

# target to preprocess a source file
src/pipelineconfig.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/pipelineconfig.c.i
.PHONY : src/pipelineconfig.c.i

src/pipelineconfig.s: src/pipelineconfig.c.s
.PHONY : src/pipelineconfig.s

# target to generate assembly for a file
src/pipelineconfig.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/pipelineconfig.c.s
.PHONY : src/pipelineconfig.c.s

src/renderresources.o: src/renderresources.c.o
.PHONY : src/renderresources.o

# target to build an object file
src/renderresources.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/renderresources.c.o
.PHONY : src/renderresources.c.o

src/renderresources.i: src/renderresources.c.i
.PHONY : src/renderresources.i

# target to preprocess a source file
src/renderresources.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/renderresources.c.i
.PHONY : src/renderresources.c.i

src/renderresources.s: src/renderresources.c.s
.PHONY : src/renderresources.s

# target to generate assembly for a file
src/renderresources.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/renderresources.c.s
.PHONY : src/renderresources.c.s

src/sound.o: src/sound.c.o
.PHONY : src/sound.o

# target to build an object file
src/sound.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/sound.c.o
.PHONY : src/sound.c.o

src/sound.i: src/sound.c.i
.PHONY : src/sound.i

# target to preprocess a source file
src/sound.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/sound.c.i
.PHONY : src/sound.c.i

src/sound.s: src/sound.c.s
.PHONY : src/sound.s

# target to generate assembly for a file
src/sound.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/sound.c.s
.PHONY : src/sound.c.s

src/ubo_update.o: src/ubo_update.c.o
.PHONY : src/ubo_update.o

# target to build an object file
src/ubo_update.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ubo_update.c.o
.PHONY : src/ubo_update.c.o

src/ubo_update.i: src/ubo_update.c.i
.PHONY : src/ubo_update.i

# target to preprocess a source file
src/ubo_update.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ubo_update.c.i
.PHONY : src/ubo_update.c.i

src/ubo_update.s: src/ubo_update.c.s
.PHONY : src/ubo_update.s

# target to generate assembly for a file
src/ubo_update.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/ubo_update.c.s
.PHONY : src/ubo_update.c.s

src/util_strings.o: src/util_strings.c.o
.PHONY : src/util_strings.o

# target to build an object file
src/util_strings.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/util_strings.c.o
.PHONY : src/util_strings.c.o

src/util_strings.i: src/util_strings.c.i
.PHONY : src/util_strings.i

# target to preprocess a source file
src/util_strings.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/util_strings.c.i
.PHONY : src/util_strings.c.i

src/util_strings.s: src/util_strings.c.s
.PHONY : src/util_strings.s

# target to generate assembly for a file
src/util_strings.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/util_strings.c.s
.PHONY : src/util_strings.c.s

src/vulkan/buffer.o: src/vulkan/buffer.c.o
.PHONY : src/vulkan/buffer.o

# target to build an object file
src/vulkan/buffer.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/buffer.c.o
.PHONY : src/vulkan/buffer.c.o

src/vulkan/buffer.i: src/vulkan/buffer.c.i
.PHONY : src/vulkan/buffer.i

# target to preprocess a source file
src/vulkan/buffer.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/buffer.c.i
.PHONY : src/vulkan/buffer.c.i

src/vulkan/buffer.s: src/vulkan/buffer.c.s
.PHONY : src/vulkan/buffer.s

# target to generate assembly for a file
src/vulkan/buffer.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/buffer.c.s
.PHONY : src/vulkan/buffer.c.s

src/vulkan/commandbuffer.o: src/vulkan/commandbuffer.c.o
.PHONY : src/vulkan/commandbuffer.o

# target to build an object file
src/vulkan/commandbuffer.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandbuffer.c.o
.PHONY : src/vulkan/commandbuffer.c.o

src/vulkan/commandbuffer.i: src/vulkan/commandbuffer.c.i
.PHONY : src/vulkan/commandbuffer.i

# target to preprocess a source file
src/vulkan/commandbuffer.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandbuffer.c.i
.PHONY : src/vulkan/commandbuffer.c.i

src/vulkan/commandbuffer.s: src/vulkan/commandbuffer.c.s
.PHONY : src/vulkan/commandbuffer.s

# target to generate assembly for a file
src/vulkan/commandbuffer.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandbuffer.c.s
.PHONY : src/vulkan/commandbuffer.c.s

src/vulkan/commandpool.o: src/vulkan/commandpool.c.o
.PHONY : src/vulkan/commandpool.o

# target to build an object file
src/vulkan/commandpool.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandpool.c.o
.PHONY : src/vulkan/commandpool.c.o

src/vulkan/commandpool.i: src/vulkan/commandpool.c.i
.PHONY : src/vulkan/commandpool.i

# target to preprocess a source file
src/vulkan/commandpool.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandpool.c.i
.PHONY : src/vulkan/commandpool.c.i

src/vulkan/commandpool.s: src/vulkan/commandpool.c.s
.PHONY : src/vulkan/commandpool.s

# target to generate assembly for a file
src/vulkan/commandpool.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/commandpool.c.s
.PHONY : src/vulkan/commandpool.c.s

src/vulkan/depth.o: src/vulkan/depth.c.o
.PHONY : src/vulkan/depth.o

# target to build an object file
src/vulkan/depth.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/depth.c.o
.PHONY : src/vulkan/depth.c.o

src/vulkan/depth.i: src/vulkan/depth.c.i
.PHONY : src/vulkan/depth.i

# target to preprocess a source file
src/vulkan/depth.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/depth.c.i
.PHONY : src/vulkan/depth.c.i

src/vulkan/depth.s: src/vulkan/depth.c.s
.PHONY : src/vulkan/depth.s

# target to generate assembly for a file
src/vulkan/depth.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/depth.c.s
.PHONY : src/vulkan/depth.c.s

src/vulkan/descriptor.o: src/vulkan/descriptor.c.o
.PHONY : src/vulkan/descriptor.o

# target to build an object file
src/vulkan/descriptor.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/descriptor.c.o
.PHONY : src/vulkan/descriptor.c.o

src/vulkan/descriptor.i: src/vulkan/descriptor.c.i
.PHONY : src/vulkan/descriptor.i

# target to preprocess a source file
src/vulkan/descriptor.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/descriptor.c.i
.PHONY : src/vulkan/descriptor.c.i

src/vulkan/descriptor.s: src/vulkan/descriptor.c.s
.PHONY : src/vulkan/descriptor.s

# target to generate assembly for a file
src/vulkan/descriptor.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/descriptor.c.s
.PHONY : src/vulkan/descriptor.c.s

src/vulkan/framebuffer.o: src/vulkan/framebuffer.c.o
.PHONY : src/vulkan/framebuffer.o

# target to build an object file
src/vulkan/framebuffer.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/framebuffer.c.o
.PHONY : src/vulkan/framebuffer.c.o

src/vulkan/framebuffer.i: src/vulkan/framebuffer.c.i
.PHONY : src/vulkan/framebuffer.i

# target to preprocess a source file
src/vulkan/framebuffer.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/framebuffer.c.i
.PHONY : src/vulkan/framebuffer.c.i

src/vulkan/framebuffer.s: src/vulkan/framebuffer.c.s
.PHONY : src/vulkan/framebuffer.s

# target to generate assembly for a file
src/vulkan/framebuffer.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/framebuffer.c.s
.PHONY : src/vulkan/framebuffer.c.s

src/vulkan/image.o: src/vulkan/image.c.o
.PHONY : src/vulkan/image.o

# target to build an object file
src/vulkan/image.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/image.c.o
.PHONY : src/vulkan/image.c.o

src/vulkan/image.i: src/vulkan/image.c.i
.PHONY : src/vulkan/image.i

# target to preprocess a source file
src/vulkan/image.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/image.c.i
.PHONY : src/vulkan/image.c.i

src/vulkan/image.s: src/vulkan/image.c.s
.PHONY : src/vulkan/image.s

# target to generate assembly for a file
src/vulkan/image.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/image.c.s
.PHONY : src/vulkan/image.c.s

src/vulkan/instance.o: src/vulkan/instance.c.o
.PHONY : src/vulkan/instance.o

# target to build an object file
src/vulkan/instance.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/instance.c.o
.PHONY : src/vulkan/instance.c.o

src/vulkan/instance.i: src/vulkan/instance.c.i
.PHONY : src/vulkan/instance.i

# target to preprocess a source file
src/vulkan/instance.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/instance.c.i
.PHONY : src/vulkan/instance.c.i

src/vulkan/instance.s: src/vulkan/instance.c.s
.PHONY : src/vulkan/instance.s

# target to generate assembly for a file
src/vulkan/instance.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/instance.c.s
.PHONY : src/vulkan/instance.c.s

src/vulkan/memory.o: src/vulkan/memory.c.o
.PHONY : src/vulkan/memory.o

# target to build an object file
src/vulkan/memory.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/memory.c.o
.PHONY : src/vulkan/memory.c.o

src/vulkan/memory.i: src/vulkan/memory.c.i
.PHONY : src/vulkan/memory.i

# target to preprocess a source file
src/vulkan/memory.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/memory.c.i
.PHONY : src/vulkan/memory.c.i

src/vulkan/memory.s: src/vulkan/memory.c.s
.PHONY : src/vulkan/memory.s

# target to generate assembly for a file
src/vulkan/memory.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/memory.c.s
.PHONY : src/vulkan/memory.c.s

src/vulkan/physicaldevice.o: src/vulkan/physicaldevice.c.o
.PHONY : src/vulkan/physicaldevice.o

# target to build an object file
src/vulkan/physicaldevice.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/physicaldevice.c.o
.PHONY : src/vulkan/physicaldevice.c.o

src/vulkan/physicaldevice.i: src/vulkan/physicaldevice.c.i
.PHONY : src/vulkan/physicaldevice.i

# target to preprocess a source file
src/vulkan/physicaldevice.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/physicaldevice.c.i
.PHONY : src/vulkan/physicaldevice.c.i

src/vulkan/physicaldevice.s: src/vulkan/physicaldevice.c.s
.PHONY : src/vulkan/physicaldevice.s

# target to generate assembly for a file
src/vulkan/physicaldevice.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/physicaldevice.c.s
.PHONY : src/vulkan/physicaldevice.c.s

src/vulkan/pipeline.o: src/vulkan/pipeline.c.o
.PHONY : src/vulkan/pipeline.o

# target to build an object file
src/vulkan/pipeline.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/pipeline.c.o
.PHONY : src/vulkan/pipeline.c.o

src/vulkan/pipeline.i: src/vulkan/pipeline.c.i
.PHONY : src/vulkan/pipeline.i

# target to preprocess a source file
src/vulkan/pipeline.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/pipeline.c.i
.PHONY : src/vulkan/pipeline.c.i

src/vulkan/pipeline.s: src/vulkan/pipeline.c.s
.PHONY : src/vulkan/pipeline.s

# target to generate assembly for a file
src/vulkan/pipeline.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/pipeline.c.s
.PHONY : src/vulkan/pipeline.c.s

src/vulkan/render.o: src/vulkan/render.c.o
.PHONY : src/vulkan/render.o

# target to build an object file
src/vulkan/render.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/render.c.o
.PHONY : src/vulkan/render.c.o

src/vulkan/render.i: src/vulkan/render.c.i
.PHONY : src/vulkan/render.i

# target to preprocess a source file
src/vulkan/render.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/render.c.i
.PHONY : src/vulkan/render.c.i

src/vulkan/render.s: src/vulkan/render.c.s
.PHONY : src/vulkan/render.s

# target to generate assembly for a file
src/vulkan/render.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/render.c.s
.PHONY : src/vulkan/render.c.s

src/vulkan/renderpass.o: src/vulkan/renderpass.c.o
.PHONY : src/vulkan/renderpass.o

# target to build an object file
src/vulkan/renderpass.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/renderpass.c.o
.PHONY : src/vulkan/renderpass.c.o

src/vulkan/renderpass.i: src/vulkan/renderpass.c.i
.PHONY : src/vulkan/renderpass.i

# target to preprocess a source file
src/vulkan/renderpass.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/renderpass.c.i
.PHONY : src/vulkan/renderpass.c.i

src/vulkan/renderpass.s: src/vulkan/renderpass.c.s
.PHONY : src/vulkan/renderpass.s

# target to generate assembly for a file
src/vulkan/renderpass.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/renderpass.c.s
.PHONY : src/vulkan/renderpass.c.s

src/vulkan/sampler.o: src/vulkan/sampler.c.o
.PHONY : src/vulkan/sampler.o

# target to build an object file
src/vulkan/sampler.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/sampler.c.o
.PHONY : src/vulkan/sampler.c.o

src/vulkan/sampler.i: src/vulkan/sampler.c.i
.PHONY : src/vulkan/sampler.i

# target to preprocess a source file
src/vulkan/sampler.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/sampler.c.i
.PHONY : src/vulkan/sampler.c.i

src/vulkan/sampler.s: src/vulkan/sampler.c.s
.PHONY : src/vulkan/sampler.s

# target to generate assembly for a file
src/vulkan/sampler.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/sampler.c.s
.PHONY : src/vulkan/sampler.c.s

src/vulkan/setup.o: src/vulkan/setup.c.o
.PHONY : src/vulkan/setup.o

# target to build an object file
src/vulkan/setup.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/setup.c.o
.PHONY : src/vulkan/setup.c.o

src/vulkan/setup.i: src/vulkan/setup.c.i
.PHONY : src/vulkan/setup.i

# target to preprocess a source file
src/vulkan/setup.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/setup.c.i
.PHONY : src/vulkan/setup.c.i

src/vulkan/setup.s: src/vulkan/setup.c.s
.PHONY : src/vulkan/setup.s

# target to generate assembly for a file
src/vulkan/setup.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/setup.c.s
.PHONY : src/vulkan/setup.c.s

src/vulkan/shaders.o: src/vulkan/shaders.c.o
.PHONY : src/vulkan/shaders.o

# target to build an object file
src/vulkan/shaders.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/shaders.c.o
.PHONY : src/vulkan/shaders.c.o

src/vulkan/shaders.i: src/vulkan/shaders.c.i
.PHONY : src/vulkan/shaders.i

# target to preprocess a source file
src/vulkan/shaders.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/shaders.c.i
.PHONY : src/vulkan/shaders.c.i

src/vulkan/shaders.s: src/vulkan/shaders.c.s
.PHONY : src/vulkan/shaders.s

# target to generate assembly for a file
src/vulkan/shaders.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/shaders.c.s
.PHONY : src/vulkan/shaders.c.s

src/vulkan/swapchain.o: src/vulkan/swapchain.c.o
.PHONY : src/vulkan/swapchain.o

# target to build an object file
src/vulkan/swapchain.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/swapchain.c.o
.PHONY : src/vulkan/swapchain.c.o

src/vulkan/swapchain.i: src/vulkan/swapchain.c.i
.PHONY : src/vulkan/swapchain.i

# target to preprocess a source file
src/vulkan/swapchain.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/swapchain.c.i
.PHONY : src/vulkan/swapchain.c.i

src/vulkan/swapchain.s: src/vulkan/swapchain.c.s
.PHONY : src/vulkan/swapchain.s

# target to generate assembly for a file
src/vulkan/swapchain.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/vulkan/swapchain.c.s
.PHONY : src/vulkan/swapchain.c.s

src/window.o: src/window.c.o
.PHONY : src/window.o

# target to build an object file
src/window.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/window.c.o
.PHONY : src/window.c.o

src/window.i: src/window.c.i
.PHONY : src/window.i

# target to preprocess a source file
src/window.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/window.c.i
.PHONY : src/window.c.i

src/window.s: src/window.c.s
.PHONY : src/window.s

# target to generate assembly for a file
src/window.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/src/window.c.s
.PHONY : src/window.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Game"
	@echo "... src/camera.o"
	@echo "... src/camera.i"
	@echo "... src/camera.s"
	@echo "... src/imagememory.o"
	@echo "... src/imagememory.i"
	@echo "... src/imagememory.s"
	@echo "... src/kotlin.o"
	@echo "... src/kotlin.i"
	@echo "... src/kotlin.s"
	@echo "... src/light.o"
	@echo "... src/light.i"
	@echo "... src/light.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
	@echo "... src/modeldata.o"
	@echo "... src/modeldata.i"
	@echo "... src/modeldata.s"
	@echo "... src/pipelineconfig.o"
	@echo "... src/pipelineconfig.i"
	@echo "... src/pipelineconfig.s"
	@echo "... src/renderresources.o"
	@echo "... src/renderresources.i"
	@echo "... src/renderresources.s"
	@echo "... src/sound.o"
	@echo "... src/sound.i"
	@echo "... src/sound.s"
	@echo "... src/ubo_update.o"
	@echo "... src/ubo_update.i"
	@echo "... src/ubo_update.s"
	@echo "... src/util_strings.o"
	@echo "... src/util_strings.i"
	@echo "... src/util_strings.s"
	@echo "... src/vulkan/buffer.o"
	@echo "... src/vulkan/buffer.i"
	@echo "... src/vulkan/buffer.s"
	@echo "... src/vulkan/commandbuffer.o"
	@echo "... src/vulkan/commandbuffer.i"
	@echo "... src/vulkan/commandbuffer.s"
	@echo "... src/vulkan/commandpool.o"
	@echo "... src/vulkan/commandpool.i"
	@echo "... src/vulkan/commandpool.s"
	@echo "... src/vulkan/depth.o"
	@echo "... src/vulkan/depth.i"
	@echo "... src/vulkan/depth.s"
	@echo "... src/vulkan/descriptor.o"
	@echo "... src/vulkan/descriptor.i"
	@echo "... src/vulkan/descriptor.s"
	@echo "... src/vulkan/framebuffer.o"
	@echo "... src/vulkan/framebuffer.i"
	@echo "... src/vulkan/framebuffer.s"
	@echo "... src/vulkan/image.o"
	@echo "... src/vulkan/image.i"
	@echo "... src/vulkan/image.s"
	@echo "... src/vulkan/instance.o"
	@echo "... src/vulkan/instance.i"
	@echo "... src/vulkan/instance.s"
	@echo "... src/vulkan/memory.o"
	@echo "... src/vulkan/memory.i"
	@echo "... src/vulkan/memory.s"
	@echo "... src/vulkan/physicaldevice.o"
	@echo "... src/vulkan/physicaldevice.i"
	@echo "... src/vulkan/physicaldevice.s"
	@echo "... src/vulkan/pipeline.o"
	@echo "... src/vulkan/pipeline.i"
	@echo "... src/vulkan/pipeline.s"
	@echo "... src/vulkan/render.o"
	@echo "... src/vulkan/render.i"
	@echo "... src/vulkan/render.s"
	@echo "... src/vulkan/renderpass.o"
	@echo "... src/vulkan/renderpass.i"
	@echo "... src/vulkan/renderpass.s"
	@echo "... src/vulkan/sampler.o"
	@echo "... src/vulkan/sampler.i"
	@echo "... src/vulkan/sampler.s"
	@echo "... src/vulkan/setup.o"
	@echo "... src/vulkan/setup.i"
	@echo "... src/vulkan/setup.s"
	@echo "... src/vulkan/shaders.o"
	@echo "... src/vulkan/shaders.i"
	@echo "... src/vulkan/shaders.s"
	@echo "... src/vulkan/swapchain.o"
	@echo "... src/vulkan/swapchain.i"
	@echo "... src/vulkan/swapchain.s"
	@echo "... src/window.o"
	@echo "... src/window.i"
	@echo "... src/window.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

