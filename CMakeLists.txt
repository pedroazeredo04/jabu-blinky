## Define versão mínima do cmake para compilar o projeto
cmake_minimum_required(VERSION 3.16)

## Seleciona a caixa de ferramentas (toolchain) para STM32
# Should be set before calling to project()
set(CMAKE_TOOLCHAIN_FILE /home/pedrin_dj/stm32-cmake/cmake/stm32_gcc.cmake)
set(CMAKE_C_STANDARD 11)

project(jabu-blinky C ASM)

stm32_fetch_cube(F4)

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

## Declara uma biblioteca com todos os arquivos gerados pelo Cube
add_library(cube_lib
    ${CUBE_SOURCES}
)

add_library(jabu_lib
    ${JABU_SOURCES}
)

## Inclui os headers de configuração no cabeçalho de todos os arquivos

## Declara as dependências da biblioteca jabu_lib
target_link_libraries(jabu_lib
    cube_lib
    HAL::STM32::F4

    HAL::STM32::F4::ADC
    HAL::STM32::F4::CAN
    HAL::STM32::F4::CEC
    HAL::STM32::F4::CORTEX
    HAL::STM32::F4::CRC
    HAL::STM32::F4::CRYP
    HAL::STM32::F4::DAC
    HAL::STM32::F4::DCMI
    HAL::STM32::F4::DFSDM
    HAL::STM32::F4::DMA
    HAL::STM32::F4::DMA2D
    HAL::STM32::F4::DSI
    HAL::STM32::F4::ETH
    HAL::STM32::F4::EXTI
    HAL::STM32::F4::FLASH
    HAL::STM32::F4::FMPI2C
    HAL::STM32::F4::FMPSMBUS
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::HASH
    HAL::STM32::F4::HCD
    HAL::STM32::F4::I2C
    HAL::STM32::F4::I2S
    HAL::STM32::F4::IRDA
    HAL::STM32::F4::IWDG
    HAL::STM32::F4::LPTIM
    HAL::STM32::F4::LTDC
    HAL::STM32::F4::MMC
    HAL::STM32::F4::NAND
    HAL::STM32::F4::NOR
    HAL::STM32::F4::PCCARD
    HAL::STM32::F4::PCD
    HAL::STM32::F4::PWR
    HAL::STM32::F4::QSPI
    HAL::STM32::F4::RCC
    HAL::STM32::F4::RNG
    HAL::STM32::F4::RTC
    HAL::STM32::F4::SAI
    HAL::STM32::F4::SD
    HAL::STM32::F4::SDRAM
    HAL::STM32::F4::SMARTCARD
    HAL::STM32::F4::SMBUS
    HAL::STM32::F4::SPDIFRX
    HAL::STM32::F4::SPI
    HAL::STM32::F4::SRAM
    HAL::STM32::F4::TIM
    HAL::STM32::F4::UART
    HAL::STM32::F4::USART
    HAL::STM32::F4::WWDG

    HAL::STM32::F4::LL_ADC
    HAL::STM32::F4::LL_CRC
    HAL::STM32::F4::LL_DAC
    HAL::STM32::F4::LL_DMA
    HAL::STM32::F4::LL_DMA2D
    HAL::STM32::F4::LL_EXTI
    HAL::STM32::F4::LL_FMC
    HAL::STM32::F4::LL_FMPI2C
    HAL::STM32::F4::LL_FSMC
    HAL::STM32::F4::LL_GPIO
    HAL::STM32::F4::LL_I2C
    HAL::STM32::F4::LL_LPTIM
    HAL::STM32::F4::LL_PWR
    HAL::STM32::F4::LL_RCC
    HAL::STM32::F4::LL_RNG
    HAL::STM32::F4::LL_RTC
    HAL::STM32::F4::LL_SDMMC
    HAL::STM32::F4::LL_SPI
    HAL::STM32::F4::LL_TIM
    HAL::STM32::F4::LL_USART
    HAL::STM32::F4::LL_USB
    HAL::STM32::F4::LL_UTILS

    CMSIS::STM32::F411RE

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
