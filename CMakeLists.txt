# Name: Config CMake file
# ThundeRatz Robotics Team
# 03/2023

cmake_minimum_required(VERSION 3.16)

###############################################################################
## CMake Configuration
###############################################################################

set(CMAKE_TOOLCHAIN_FILE lib/stm32-cmake/cmake/stm32_gcc.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

project(jabu-blinky C ASM)

set(BOARD_VERSION "")

set(DEVICE_CORTEX F4)
set(DEVICE_FAMILY STM32F4xx)
set(DEVICE_TYPE STM32F411xx)
set(DEVICE_DEF STM32F411xE)
set(DEVICE STM32F411RE)

if(BOARD_VERSION STREQUAL "")
    set(PROJECT_RELEASE ${PROJECT_NAME})
else()
    set(PROJECT_RELEASE ${PROJECT_NAME}_${BOARD_VERSION})
endif()

# Project Include Target
set(TARGET_BOARD target_${PROJECT_RELEASE})

set(C_INCLUDE_DIRECTORIES
    ./inc
    ./cube/Inc
)

set(TEST_INCLUDE_DIRECTORIES
    ./tests/inc
)

set(FORCED_INCLUDE_DIRECTORIES
    ""
)

# Used to `make vs_files` target
set(COMPILE_DEFINITIONS
    ${DEVICE_DEF}
    USE_HAL_DRIVER
)

include(cmake/thunder.cmake)
include(cmake/vs_files.cmake)

set(DEPENDENCIES
    m  # math library

    HAL::STM32::${DEVICE_CORTEX}

    HAL::STM32::${DEVICE_CORTEX}::ADCEx
    HAL::STM32::${DEVICE_CORTEX}::GPIO

    HAL::STM32::${DEVICE_CORTEX}::RCCEx
    HAL::STM32::${DEVICE_CORTEX}::CORTEX

    CMSIS::STM32::${DEVICE_FAMILY_COMPACT}
    STM32::NoSys
)

set(LIB_SOURCES
    ""
)

###############################################################################
## Input files
###############################################################################

file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE C_HEADERS "inc/*.h")
file(GLOB_RECURSE CUBE_SOURCES "cube/Src/*.c")

# Remove system file from CubeMX (as it is already included in CMSIS)
string(TOLOWER ${DEVICE_FAMILY} DEVICE_FAMILY_LOWER)
list(REMOVE_ITEM CUBE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cube/Src/system_${DEVICE_FAMILY_LOWER}.c)

file(GLOB_RECURSE TESTS_SOURCES "tests/src/*.c")
file(GLOB_RECURSE TESTS_HEADERS "tests/inc/*.h")
file(GLOB_RECURSE TESTS_BIN "tests/bin/*.c")

###############################################################################
## Compile Definitions
###############################################################################

find_package(CMSIS COMPONENTS ${DEVICE} REQUIRED)
find_package(HAL COMPONENTS STM32${DEVICE_CORTEX} REQUIRED)

add_compile_options(
    -Wall
    -Wextra
    -Wfatal-errors
    -mthumb
    -fdata-sections
    -ffunction-sections
    -fmessage-length=0
    -MMD
    -MP
)

add_compile_definitions(${DEVICE_DEF})
add_compile_definitions(USE_HAL_DRIVER)

add_executable(${PROJECT_NAME}
    ${CUBE_SOURCES}
    ${C_SOURCES}
    ${LIB_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${C_INCLUDE_DIRECTORIES}
    ${CMSIS_INCLUDE_DIRS}
    ${HAL_INCLUDE_DIRS}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${FORCED_INCLUDE_DIRECTORIES}
)

target_link_libraries(${PROJECT_NAME}
    ${DEPENDENCIES}
)

## Add linker flags
target_link_options(${PROJECT_NAME} PUBLIC
    --specs=nano.specs
)

###############################################################################
## Generate binary files
###############################################################################

thunder_flash_target(${PROJECT_NAME})

stm32_print_size_of_target(${PROJECT_NAME})

stm32_generate_hex_file(${PROJECT_NAME})

## Find all test files and saves it at TEST_SOURCES list

foreach(TEST_FILE ${TESTS_SOURCES})
    ## If TEST_FILE contains /dir1/dir2/file.c, TEST_NAME will be 'file'
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WLE)

    ## Declares an executable TEST_NAME
    add_executable(${TEST_NAME}
        ${TEST_FILE}
        ${CUBE_SOURCES}
        ${C_SOURCES}
        ${LIB_SOURCES}
    )

    target_include_directories(${TEST_NAME} PUBLIC
        ${C_INCLUDE_DIRECTORIES}
        ${CMSIS_INCLUDE_DIRS}
        ${HAL_INCLUDE_DIRS}
        ${TEST_INCLUDE_DIRECTORIES}
    )

    target_precompile_headers(${TEST_NAME} PUBLIC
        ${FORCED_INCLUDE_DIRECTORIES}
    )

    ## Declares test dependencies
    target_link_libraries(${TEST_NAME}
        ${DEPENDENCIES}
    )

    ## Add linker flags
    target_link_options(${TEST_NAME} PUBLIC
        --specs=nano.specs
    )

    stm32_generate_hex_file(${TEST_NAME})

    thunder_flash_target(${TEST_NAME})

endforeach()
