# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET NuMaker-mbed-CustomBoard-example)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET})

# Provide Mbed OS with the header file it needs to configure Mbed TLS for LoRa
target_include_directories(${APP_TARGET}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${APP_TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
)

# Custom target


if("NUMAKER_M483KG" IN_LIST MBED_TARGET_LABELS)
    add_library(mbed-numaker-m483kg INTERFACE)
    target_include_directories(mbed-numaker-m483kg
        INTERFACE
            TARGET_NUMAKER_M483KG
    )
    target_link_libraries(mbed-numaker-m483kg INTERFACE mbed-m480)
endif()

if("NU_IOT_M2354" IN_LIST MBED_TARGET_LABELS)
    add_library(mbed-nu-iot-m2354 INTERFACE)
    add_subdirectory(TARGET_NU_IOT_M2354)
    target_include_directories(mbed-nu-iot-m2354
        INTERFACE
            TARGET_NU_IOT_M2354
    )
    target_link_libraries(mbed-nu-iot-m2354
        INTERFACE
            mbed-m2354
            mbed-m2354-tfm
    )
endif()

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
