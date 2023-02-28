# Name: config.mk
# Author: Daniel Nery Silva de Oliveira
# ThundeRatz Robotics Team
# 06/2019

# Cube file name without .ioc extension
PROJECT_NAME = jabu-blinky
VERSION := 1

TARGET_BOARD := target_$(PROJECT_NAME)_$(VERSION)

DEVICE_FAMILY  := STM32F4xx
DEVICE_TYPE    := STM32F411xx
DEVICE_DEF     := STM32F411xE
DEVICE         := STM32F411RE

# Linker script file without .ld extension
# Find it on cube folder after code generation
DEVICE_LD_FILE := STM32F411CEUx_FLASH

# Lib dir
LIB_DIR  := lib

# Cube Directory
CUBE_DIR := cube

# Config Files Directory
CFG_DIR :=

# Tests Directory
TEST_DIR := tests

# Default values, can be set on the command line or here
DEBUG   ?= 1
VERBOSE ?= 0
TEST    ?= 0

TEST_NAME ?= test_main
