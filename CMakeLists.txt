## Define versão mínima do cmake para compilar o projeto
cmake_minimum_required(VERSION 3.16)

## Seleciona a caixa de ferramentas (toolchain) para STM32
# Should be set before calling to project()
set(CMAKE_TOOLCHAIN_FILE /home/pedrin_dj/stm32-cmake/cmake/stm32_gcc.cmake)

project(jabu-blinky C ASM)

stm32_fetch_cube(F4)

find_package(CMSIS COMPONENTS STM32F411XE REQUIRED)
find_package(HAL COMPONENTS STM32F4 REQUIRED)

add_compile_options(-Wfatal-errors -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
add_compile_definitions(STM32F411XE)

include_directories(
    inc
    cube/Inc
    ${HAL_INCLUDE_DIRS}
    ${CMSIS_INCLUDE_DIRS}
)

file(GLOB_RECURSE CUBE_SOURCES "cube/Src/*.c")
file(GLOB_RECURSE JABU_SOURCES "src/*.c")

list(REMOVE_ITEM JABU_SOURCES "src/main.c")

## Declara uma biblioteca com todos os arquivos gerados pelo Cube
add_library(cube_lib
    ${CUBE_SOURCES}
)

add_library(jabu_lib
    ${JABU_SOURCES}
)

## Inclui os headers de configuração no cabeçalho de todos os arquivos
target_precompile_headers(jabu_lib PUBLIC
    ./cube/Inc/stm32f4xx_hal_conf.h
)

## Declara as dependências da biblioteca jabu_lib
target_link_libraries(jabu_lib
    cube_lib
    HAL::STM32::F4
    HAL::STM32::F4::RCC
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::CORTEX
    CMSIS::STM32::F411xE
    STM32::NoSys
)

## Adiciona flags para o linker
target_link_options(jabu_lib PUBLIC
    --specs=rdimon.specs
)

## Declara o executável que queremos gerar com o arquivo main.c
add_executable(jabu-blinky
    ./src/main.c
)

## Declara as dependências do executável jabu
target_link_libraries(jabu-blinky
    jabu_lib
)

## Encontra todos os arquivos de teste e salva na variável TESTS_SOURCES
file(GLOB_RECURSE TESTS_SOURCES "tests/src/*.c")

foreach(TEST_FILE ${TESTS_SOURCES})
    ## Declara a variável TEST_NAME como o nome do arquivo contido em TEST_FILE
    ## Se TEST_FILE contém /pasta1/pasta2/arquivo.c, TEST_NAME será 'arquivo'
    get_filename_component(TEST_NAME ${TEST_FILE} NAME)

    ## Declara um executável de nome TEST_NAME com o arquivo TEST_FILE
    add_executable(${TEST_NAME}
        ${TEST_FILE}
    )

    ## Adiciona a pasta include dos testes na lista de caminho a procurar arquivos
    target_include_directories(${TEST_NAME} PUBLIC
        ./tests/inc
    )

    ## Declara as dependências do teste
    target_link_libraries(${TEST_NAME}
        jabu_lib
    )
endforeach()

stm32_print_size_of_target(jabu-blinky)

stm32_generate_hex_file(jabu-blinky)
