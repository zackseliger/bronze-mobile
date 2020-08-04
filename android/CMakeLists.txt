#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH) # get the path to the parent folder we're in

# zlib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/zlib Z_SOURCES)
add_library(libz STATIC ${Z_SOURCES})
target_include_directories(libz PRIVATE ../lib/zlib)

# libpng
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_definitions(-DPNG_ARM_NEON_OPT=0)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/libpng PNG_SOURCES)
add_library(png STATIC ${PNG_SOURCES})
target_link_libraries(png libz)
target_include_directories(png PRIVATE ../lib/libpng)

# build common game files as a static lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/common GAME_SOURCES) # all source files in common/
add_library(game_common STATIC ${GAME_SOURCES})
# lib dependencies
target_link_libraries(game_common GLESv2 png)
# includes
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/libpng)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/common)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")
# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native-activity SHARED app/src/main/cpp/main.cpp)

# NDK and common .h files
target_include_directories(native-activity PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
    ../common)

# add lib dependencies
target_link_libraries(native-activity
    android
    game_common
    native_app_glue
    EGL
    log)