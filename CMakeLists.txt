# Name: CMakeLists file
# ThundeRatz Robotics Team
# Brief: This file contains the configuration of the CMake project
## and all the files that you should edit to configure your project
# 04/2023

cmake_minimum_required(VERSION 3.16)

###############################################################################
## CMake Configuration
###############################################################################

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lib/stm32-cmake/cmake/stm32_gcc.cmake)
set(CMAKE_C_STANDARD 11)

# Gets all directories from /lib path
file(GLOB V_GLOB LIST_DIRECTORIES true "lib/*")
foreach(item ${V_GLOB})
	if(IS_DIRECTORY ${item})
		list(APPEND CMAKE_MODULE_PATH ${item}/cmake)
	endif()
endforeach()

# Debug, Release, RelWithDebInfo and MinSizeRel
# @see: https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html
set(CMAKE_BUILD_TYPE Debug)

project(jabu-blinky C ASM)

# Set the board version to an empty string if your board doesn't have a version
set(BOARD_VERSION "")

# Device Configuration
set(DEVICE_CORTEX F4)
set(DEVICE_FAMILY STM32F4xx)
set(DEVICE_TYPE STM32F411xx)
set(DEVICE_DEF STM32F411xE)
set(DEVICE STM32F411RE)

# DEVICE_FAMILY_COMPACT is the same as DEVICE_FAMILY but without the STM32 prefix
string(REPLACE "STM32" "" DEVICE_FAMILY_COMPACT ${DEVICE})

if(BOARD_VERSION STREQUAL "")
    set(PROJECT_RELEASE ${PROJECT_NAME})
else()
    set(PROJECT_RELEASE ${PROJECT_NAME}_${BOARD_VERSION})
endif()
set(TARGET_BOARD target_${PROJECT_RELEASE})

###############################################################################
## Global compilation config
###############################################################################

set(LAUNCH_JSON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.vscode/launch.json)
set(DEBUG_FILE_NAME ${PROJECT_NAME})

include(cmake/config_validation.cmake)
include(cmake/workspace.cmake)
include(cmake/targets.cmake)

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

add_compile_definitions(${DEVICE_DEF} USE_HAL_DRIVER)

###############################################################################
## Project dependencies
###############################################################################

set(DEPENDENCIES
    m  # math library

    HAL::STM32::${DEVICE_CORTEX}

    HAL::STM32::${DEVICE_CORTEX}::ADCEx
    HAL::STM32::${DEVICE_CORTEX}::DMA
    HAL::STM32::${DEVICE_CORTEX}::GPIO
    HAL::STM32::${DEVICE_CORTEX}::I2C
    HAL::STM32::${DEVICE_CORTEX}::SPI
    HAL::STM32::${DEVICE_CORTEX}::TIMEx
    HAL::STM32::${DEVICE_CORTEX}::UART
    HAL::STM32::${DEVICE_CORTEX}::WWDG

    HAL::STM32::${DEVICE_CORTEX}::RCCEx
    HAL::STM32::${DEVICE_CORTEX}::CORTEX

    CMSIS::STM32::${DEVICE_FAMILY_COMPACT}
)

###############################################################################
## Include directories
###############################################################################

set(C_INCLUDE_DIRECTORIES
    ./inc
    ./cube/Inc
)

set(TEST_INCLUDE_DIRECTORIES
    ./tests/inc
)

###############################################################################
## Input files
###############################################################################

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS "src/*.c")
file(GLOB_RECURSE C_HEADERS CONFIGURE_DEPENDS "inc/*.h")

file(GLOB_RECURSE TESTS_SOURCES CONFIGURE_DEPENDS "tests/src/*.c")
file(GLOB_RECURSE TESTS_HEADERS CONFIGURE_DEPENDS "tests/inc/*.h")
file(GLOB_RECURSE TESTS_BIN CONFIGURE_DEPENDS "tests/bin/*.c")

file(GLOB_RECURSE CUBE_SOURCES CONFIGURE_DEPENDS "cube/Src/*.c")

# Remove system_xxx.c file from CubeMX, as it is already included in CMSIS
string(TOLOWER ${DEVICE_FAMILY} DEVICE_FAMILY_LOWER)
list(REMOVE_ITEM CUBE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cube/Src/system_${DEVICE_FAMILY_LOWER}.c)

# FORCED_INCLUDE_HEADERS will be included in all files
set(FORCED_INCLUDE_HEADERS
)

###############################################################################
## Main executable target
###############################################################################

add_executable(${PROJECT_NAME}
    ${CUBE_SOURCES}
    ${C_SOURCES}
    ${LIB_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${C_INCLUDE_DIRECTORIES}
    ${LIB_INCLUDE_DIRECTORIES}
    ${CMSIS_INCLUDE_DIRS}
    ${HAL_INCLUDE_DIRS}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC
    ${FORCED_INCLUDE_HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    ${DEPENDENCIES}
)

target_link_options(${PROJECT_NAME} PUBLIC
    --specs=nano.specs
)

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_hex_file(${PROJECT_NAME})

targets_generate_format_target(C_SOURCES C_HEADERS TESTS_SOURCES TESTS_HEADERS TESTS_BIN)
targets_generate_vsfiles_target(${PROJECT_NAME})
targets_generate_flash_target(${PROJECT_NAME})
targets_generate_helpme_target()

###############################################################################
## Generate test executables
###############################################################################

# Since each test has its own main function, we don't need the main.c from the project
list(REMOVE_ITEM C_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

foreach(TEST_FILE ${TESTS_BIN})

    # If TEST_FILE contains /dir1/dir2/file.c, TEST_NAME will be 'file'
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WLE)

    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL
        ${TEST_FILE}
        ${CUBE_SOURCES}
        ${C_SOURCES}
        ${TESTS_SOURCES}
        ${LIB_SOURCES}
    )

    target_include_directories(${TEST_NAME} PUBLIC
        ${C_INCLUDE_DIRECTORIES}
        ${LIB_INCLUDE_DIRECTORIES}
        ${CMSIS_INCLUDE_DIRS}
        ${HAL_INCLUDE_DIRS}
        ${TEST_INCLUDE_DIRECTORIES}
    )

    target_precompile_headers(${TEST_NAME} PUBLIC
        ${FORCED_INCLUDE_HEADERS}
    )

    target_link_libraries(${TEST_NAME}
        ${DEPENDENCIES}
    )

    target_link_options(${TEST_NAME} PUBLIC
        --specs=nano.specs
    )

    stm32_generate_hex_file(${TEST_NAME})

    targets_generate_vsfiles_target(${TEST_NAME})
    targets_generate_flash_target(${TEST_NAME})

endforeach()
