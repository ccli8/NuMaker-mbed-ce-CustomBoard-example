# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

set(MBED_OS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")

# Set path for mbed_app.json5
set(MBED_APP_JSON_PATH mbed_app.json5)

# Set path for custom_targets.json5
set(CUSTOM_TARGETS_PATH custom_targets) 
set(CUSTOM_TARGETS_JSON_PATH ${CUSTOM_TARGETS_PATH}/custom_targets.json5) 

# Load Mbed CE toolchain file and basic build system
include(${MBED_OS_PATH}/tools/cmake/app.cmake)

set(APP_PROJECT NUMAKER_MBED_CE_CUSTOMBOARD_EXAMPLE)
set(APP_TARGET NuMaker-mbed-ce-CustomBoard-example)

# Set up project name
project(${APP_PROJECT})

# Add Nuvoton CMake list files to CMake default module path
#
# Normally, this is added in ${MBED_OS_PATH}, but ${CUSTOM_TARGETS_PATH}
# is in front and needs it.
list(APPEND CMAKE_MODULE_PATH
    ${MBED_OS_PATH}/targets/TARGET_NUVOTON/scripts
)

# Add for custom targets
add_subdirectory(${CUSTOM_TARGETS_PATH})

# Add Mbed OS library
add_subdirectory(${MBED_OS_PATH})

# Add RHE6616TP01 LCD
if("COMPONENT_RHE6616TP01_LCD=1" IN_LIST MBED_TARGET_DEFINITIONS)
    add_subdirectory(drivers/COMPONENT_RHE6616TP01_LCD)
endif()

add_executable(${APP_TARGET})

target_sources(${APP_TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
        $<$<IN_LIST:COMPONENT_RHE6616TP01_LCD=1,${MBED_TARGET_DEFINITIONS}>:rhe6616tp01-lcd>
)

# Must call this for each target to set up bin file creation, code upload, etc
mbed_set_post_build(${APP_TARGET})

# Make sure this is the last line of the top-level build script
mbed_finalize_build()
