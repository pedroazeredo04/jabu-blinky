## Define versão mínima do cmake para compilar o projeto
cmake_minimum_required(VERSION 3.16)

## Seleciona a caixa de ferramentas (toolchain) para STM32
set(CMAKE_TOOLCHAIN_FILE ./lib/stm32-cmake/cmake/stm32_gcc.cmake)
set(CMAKE_C_STANDARD 11)

## Define variáveis para a configuração do projeto
set(STM32_CUBE_F4_PATH ./cube)
set(CUBE_DIR ./cube)
set(BUILD_DIR ./build)

## Define variáveis para a configuração do STM32CubeMX
if(DEFINED ENV{CUBE_PATH})
    message(STATUS "CUBE_PATH defined as $ENV{CUBE_PATH}")
    set(JAVA_EXE $ENV{CUBE_PATH}/jre/bin/java)
    set(CUBE_JAR $ENV{CUBE_PATH}/STM32CubeMX)
else()
    message(FATAL_ERROR "CUBE_PATH not defined")
endif()

# Checa se o diretório cube está vazio ou não
file(GLOB DRIVERS ${CUBE_DIR}/Src/*.c)
list(LENGTH DRIVERS DRIVERS_LENGTH)
if(DRIVERS_LENGTH EQUAL 0)
    message(STATUS "Cube directory is empty. Generating cube files...")

    file(WRITE ${BUILD_DIR}/makecube.txt
        "config load ../${CUBE_DIR}/jabu-blinky.ioc\n"
        "project generate\n"
        "exit\n"
    )

    execute_process(COMMAND ${JAVA_EXE} -jar ${CUBE_JAR} -q makecube.txt)
endif()

# Registra o comando make flash
add_custom_target(flash
    COMMAND -E echo "Flashing..."
    COMMAND STM32_Programmer_CLI -c port=SWD -w ${PROJECT_NAME}.hex -v -rst
)

# Registra o comando make clean_all
add_custom_target(clean_all
    COMMAND -E echo "Cleaning all build files..."
    COMMAND rm -rf ../${BUILD_DIR}
)

# Registra o comando make cube
add_custom_target(cube
    file(WRITE ${BUILD_DIR}/makecube.txt
        "config load ../${CUBE_DIR}/jabu-blinky.ioc\n"
        "project generate\n"
        "exit\n"
    )

    execute_process(COMMAND ${JAVA_EXE} -jar ${CUBE_JAR} -q makecube.txt)
)

# Registra o comando make clean_cube
add_custom_target(clean_cube
    COMMAND -E echo "Cleaning cube files..."
    COMMAND mv ../${CUBE_DIR}/*.ioc .
    COMMAND rm -rf ../${CUBE_DIR}/
    COMMAND mkdir ../${CUBE_DIR}/
    COMMAND mv *.ioc ../${CUBE_DIR}/
)

## Define o nome do projeto
project(jabu-blinky C ASM)


set(HAL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stm32-cmake/cmake/
)

message("HAL_INCLUDE_DIRS: ${HAL_INCLUDE_DIRS}")

find_package(HAL COMPONENTS STM32F4 REQUIRED)
find_package(CMSIS COMPONENTS STM32F411RE REQUIRED)

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

file(GLOB_RECURSE JABU_SOURCES "src/*.c")

file(GLOB_RECURSE CUBE_SOURCES "cube/Src/*.c")
list(REMOVE_ITEM CUBE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cube/Src/system_stm32f4xx.c)

message("CUBE_SOURCES: ${CUBE_SOURCES}")

## Declara o executável que queremos gerar com o arquivo main.c
add_executable(jabu-blinky
    ./src/main.c
    ${CUBE_SOURCES}
    ${JABU_SOURCES}
)

## Declara as dependências do executável jabu
target_link_libraries(jabu-blinky

    HAL::STM32::F4

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


## Encontra todos os arquivos de teste e salva na variável TESTS_SOURCES
file(GLOB_RECURSE TESTS_SOURCES "tests/src/*.c")

foreach(TEST_FILE ${TESTS_SOURCES})
    ## Declara a variável TEST_NAME como o nome do arquivo contido em TEST_FILE
    ## Se TEST_FILE contém /pasta1/pasta2/arquivo.c, TEST_NAME será 'arquivo'
    get_filename_component(TEST_NAME ${TEST_FILE} NAME)

    ## Declara um executável de nome TEST_NAME com o arquivo TEST_FILE
    add_executable(${TEST_NAME}
        ${TEST_FILE}
        ${CUBE_SOURCES}
        ${JABU_SOURCES}
    )

    ## Adiciona a pasta include dos testes na lista de caminho a procurar arquivos
    target_include_directories(${TEST_NAME} PUBLIC
        ./tests/inc
    )

    ## Declara as dependências do teste
    target_link_libraries(${TEST_NAME}
        HAL::STM32::F4

        HAL::STM32::F4::GPIO
        HAL::STM32::F4::RCC
        HAL::STM32::F4::CORTEX

        CMSIS::STM32::F411RE

        STM32::NoSys
    )
endforeach()


stm32_print_size_of_target(jabu-blinky)

stm32_generate_hex_file(jabu-blinky)
