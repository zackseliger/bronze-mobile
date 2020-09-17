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

# FreeType
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_definitions(-DFT2_BUILD_LIBRARY)
add_library(freetype
        ../lib/freetype/src/winfonts/winfnt.c
        ../lib/freetype/src/type42/type42.c
        ../lib/freetype/src/type1/type1.c
        ../lib/freetype/src/truetype/truetype.c
        ../lib/freetype/src/smooth/smooth.c
        ../lib/freetype/src/sfnt/sfnt.c
        ../lib/freetype/src/raster/raster.c
        ../lib/freetype/src/psnames/psnames.c
        ../lib/freetype/src/pshinter/pshinter.c
        ../lib/freetype/src/psaux/psaux.c
        ../lib/freetype/src/pfr/pfr.c
        ../lib/freetype/src/pcf/pcf.c
        ../lib/freetype/src/lzw/ftlzw.c
        ../lib/freetype/src/gzip/ftgzip.c
        ../lib/freetype/src/cid/type1cid.c
        ../lib/freetype/src/cff/cff.c
        ../lib/freetype/src/cache/ftcache.c
        ../lib/freetype/src/bzip2/ftbzip2.c
        ../lib/freetype/src/bdf/bdf.c
        ../lib/freetype/src/base/ftglyph.c
        ../lib/freetype/src/base/ftgxval.c
        ../lib/freetype/src/base/ftinit.c
        ../lib/freetype/src/base/ftmm.c
        ../lib/freetype/src/base/ftotval.c
        ../lib/freetype/src/base/ftpatent.c
        ../lib/freetype/src/base/ftpfr.c
        ../lib/freetype/src/base/ftstroke.c
        ../lib/freetype/src/base/ftsynth.c
        ../lib/freetype/src/base/fttype1.c
        ../lib/freetype/src/base/ftwinfnt.c
        ../lib/freetype/src/base/ftbbox.c
        ../lib/freetype/src/base/ftbdf.c
        ../lib/freetype/src/base/ftbitmap.c
        ../lib/freetype/src/base/ftcid.c
        ../lib/freetype/src/base/ftdebug.c
        ../lib/freetype/src/base/ftfstype.c
        ../lib/freetype/src/base/ftgasp.c
        ../lib/freetype/src/base/ftbase.c
        ../lib/freetype/src/base/ftsystem.c
        ../lib/freetype/src/autofit/autofit.c)
target_link_libraries(freetype libz)
target_include_directories(freetype PRIVATE ../lib/freetype/include)

# Freetype-GL
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/freetypeGL FREETYPE_GL_SOURCES)
add_library(freetypeGL STATIC ${FREETYPE_GL_SOURCES})
target_link_libraries(freetypeGL freetype)
target_include_directories(freetypeGL PRIVATE ../lib/freetypeGL ../lib/freetype/include)

# sndfile
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/sndfile SNDFILE_SOURCES)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/sndfile/G72x SNDFILE_G72x)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/sndfile/GSM610 SNDFILE_GSM610)
add_library(sndfile ${SNDFILE_SOURCES}
        ${SNDFILE_G72x}
        ${SNDFILE_GSM610}
        ../lib/sndfile/ALAC/ag_dec.c
        ../lib/sndfile/ALAC/ag_enc.c
        ../lib/sndfile/ALAC/alac_decoder.c
        ../lib/sndfile/ALAC/alac_encoder.c
        ../lib/sndfile/ALAC/dp_dec.c
        ../lib/sndfile/ALAC/dp_enc.c
        ../lib/sndfile/ALAC/ALACBitUtilities.c
        ../lib/sndfile/ALAC/matrix_dec.c
        ../lib/sndfile/ALAC/matrix_enc.c)
target_include_directories(sndfile PRIVATE ../lib/sndfile)

# openal
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Werror")
add_definitions(-DAL_ALEXT_PROTOTYPES=1)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/openal AL_SOURCES)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/openal/al AL_SOURCES2)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/openal/effects AL_EFFECTS)
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/lib/openal/filters AL_FILTERS)
add_library(openal ${AL_SOURCES}
        ${AL_SOURCES2}
        ${AL_EFFECTS}
        ${AL_FILTERS}
        ../lib/openal/mixer/mixer_c.cpp
        ../lib/openal/mixer/mixer_neon.cpp
        ../lib/openal/backends/base.cpp
        ../lib/openal/backends/null.cpp
        ../lib/openal/backends/loopback.cpp
        ../lib/openal/backends/opensl.cpp)
target_include_directories(openal PRIVATE ../lib/openal/include ../lib/openal)
target_link_libraries(openal OpenSLES)

# build common game files as a static lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Werror")
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/common GAME_SOURCES) # all source files in common/
AUX_SOURCE_DIRECTORY(${PARENT_DIR}/common/assets GAME_SOURCES_ASSETS) # source files in common/assets
add_library(game_common STATIC ${GAME_SOURCES} ${GAME_SOURCES_ASSETS})
# lib dependencies
target_link_libraries(game_common GLESv2 png freetype freetypeGL sndfile openal)
# includes
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/libpng)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/freetype/include ${PARENT_DIR}/lib/freetype/src)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/freetypeGL)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/sndfile)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/lib/openal/include)
target_include_directories(game_common PUBLIC ${PARENT_DIR}/common)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wall -Werror")
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