## Define versão mínima do cmake para compilar o projeto
cmake_minimum_required(VERSION 3.16)

## Seleciona a caixa de ferramentas (toolchain) para STM32
# Should be set before calling to project()
set(CMAKE_TOOLCHAIN_FILE /home/pedrin_dj/stm32-cmake/cmake/stm32_gcc.cmake)
set(CMAKE_C_STANDARD 11)
set(STM32_CUBE_F4_PATH ./cube)

project(jabu-blinky C ASM)

find_package(CMSIS COMPONENTS STM32F411RE REQUIRED)
find_package(HAL COMPONENTS STM32F4 REQUIRED)

add_compile_options(
    -Wfatal-errors
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -mthumb
    -fdata-sections
    -ffunction-sections
    -fmessage-length=0
)

add_compile_definitions(STM32F411xE)
add_compile_definitions(USE_HAL_DRIVER)

include_directories(
    inc
    cube/Inc
    ${HAL_INCLUDE_DIRS}
    ${CMSIS_INCLUDE_DIRS}
)

file(GLOB_RECURSE CUBE_SOURCES "cube/Src/*.c")
file(GLOB_RECURSE JABU_SOURCES "src/*.c")

list(REMOVE_ITEM JABU_SOURCES "src/main.c")
list(REMOVE_ITEM CUBE_SOURCES "cube/Src/system_stm32f4xx.c")

## Declara o executável que queremos gerar com o arquivo main.c
add_executable(jabu-blinky
    ./src/main.c
    ${CUBE_SOURCES}
    ${JABU_SOURCES}
)


## Declara as dependências do executável jabu
target_link_libraries(jabu-blinky

    #HAL::STM32::F4

    HAL::STM32::F4::GPIO
    HAL::STM32::F4::RCC
    HAL::STM32::F4::CORTEX

    CMSIS::STM32::F411RE

    STM32::NoSys
)

## Adiciona flags para o linker
target_link_options(jabu-blinky PUBLIC
    --specs=nano.specs
)


# ## Encontra todos os arquivos de teste e salva na variável TESTS_SOURCES
# file(GLOB_RECURSE TESTS_SOURCES "tests/src/*.c")

# foreach(TEST_FILE ${TESTS_SOURCES})
#     ## Declara a variável TEST_NAME como o nome do arquivo contido em TEST_FILE
#     ## Se TEST_FILE contém /pasta1/pasta2/arquivo.c, TEST_NAME será 'arquivo'
#     get_filename_component(TEST_NAME ${TEST_FILE} NAME)

#     ## Declara um executável de nome TEST_NAME com o arquivo TEST_FILE
#     add_executable(${TEST_NAME}
#         ${TEST_FILE}
#     )

#     ## Adiciona a pasta include dos testes na lista de caminho a procurar arquivos
#     target_include_directories(${TEST_NAME} PUBLIC
#         ./tests/inc
#     )

#     ## Declara as dependências do teste
#     target_link_libraries(${TEST_NAME}
#         jabu_lib
#     )
# endforeach()


add_custom_target(flash
    STM32_Programmer_CLI -c port=SWD -w ${PROJECT_NAME}.hex -v -rst
)

stm32_print_size_of_target(jabu-blinky)

stm32_generate_hex_file(jabu-blinky)
