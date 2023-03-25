## Define variáveis para a configuração do STM32CubeMX
if(DEFINED ENV{CUBE_PATH})
    message(STATUS "CUBE_PATH defined as $ENV{CUBE_PATH}")
else()
    message(FATAL_ERROR "CUBE_PATH not defined")
endif()

if(CMAKE_HOST_WIN32)
    set(JAVA_EXE "$ENV{CUBE_PATH}\\STM32CubeMX.exe")
    set(CUBE_JAR "$ENV{CUBE_PATH}\\jre\\bin\\java.exe")
    set(JLINK_EXE JLink.exe)
else()
    set(JAVA_EXE $ENV{CUBE_PATH}/jre/bin/java)
    set(CUBE_JAR $ENV{CUBE_PATH}/STM32CubeMX)
    set(JLINK_EXE JLinkExe)
endif()


set(STM32_CUBE_${DEVICE_CORTEX}_PATH cube)

string(REPLACE "STM32" "" DEVICE_FAMILY_COMPACT ${DEVICE})

# ##############################################################################
# # Auxiliary Targets
# ##############################################################################

## Check cube directory for files
# If it's empty, generate the files
# It's important to do it before find_package(CMSIS)
file(GLOB_RECURSE CUBE_SOURCES "cube/Src/*.c")
list(LENGTH CUBE_SOURCES CUBE_LENGHT)
if(CUBE_LENGHT EQUAL 0)
    message(STATUS "Cube directory is empty. Generating cube files...")

    file(WRITE build/.cube
        "config load ../cube/${PROJECT_RELEASE}.ioc\n"
        "project generate\n"
        "exit\n"
    )

    execute_process(COMMAND ${JAVA_EXE} -jar ${CUBE_JAR} -q .cube)
endif()

# Help
add_custom_target(helpme
    COMMAND echo " "
    COMMAND echo "----------------------- Thunder CMake ------------------------------"
    COMMAND echo "   Boas vindas ao CMakeLists da ThundeRatz, cheque as configuracoes    "
    COMMAND echo "                atuais e mude o arquivo se necessario                 "
    COMMAND echo " "
    COMMAND echo "Opcoes:"
    COMMAND echo " helpme:        mostra essa ajuda"
    COMMAND echo " cube:          gera arquivos do cube"
    COMMAND echo " info:          mostra informações sobre o uC conectado"
    COMMAND echo " flash:         carrega os arquivos compilados no microcontrolador via st-link"
    COMMAND echo " flash_<teste>:  carrega os arquivos compilados de um teste no microcontrolador via st-link"
    COMMAND echo " jflash:        carrega os arquivos compilados no microcontrolador via j-link"
    COMMAND echo " jflash_<teste>: carrega os arquivos compilados de um teste no microcontrolador via j-link"
    COMMAND echo " format:        formata os arquivos .c/.h"
    COMMAND echo " clean_all:     limpa os arquivos compilados, inclusive bibliotecas da ST"
    COMMAND echo " clean_cube:    limpa os arquivos gerados pelo Cube"
    COMMAND echo " reset:         reseta o microcontrolador"
    COMMAND echo " "
    COMMAND echo "Configuracoes atuais:"
    COMMAND echo "	DEVICE_FAMILY  =  ${DEVICE_FAMILY}"
    COMMAND echo "	DEVICE_TYPE    =  ${DEVICE_TYPE}"
    COMMAND echo "	DEVICE         =  ${DEVICE}"
    COMMAND echo "	DEVICE_LD_FILE =  ${DEVICE_LD_FILE}"
    COMMAND echo "	DEVICE_DEF     =  ${DEVICE_DEF}"
    COMMAND echo "	"
)

# Format
add_custom_target(format
    COMMAND uncrustify -c ../uncrustify.cfg --replace --no-backup ${C_SOURCES} ${C_HEADERS} ${TESTS_HEADERS} ${TESTS_SOURCES} ${TESTS_BIN}
)

# Make Cube Files
add_custom_target(cube
    COMMAND echo "Generating cube files..."

    file(WRITE $build/.cube
        "config load ../cube/${PROJECT_RELEASE}\n"
        "project generate\n"
        "exit\n"
    )

    execute_process(COMMAND ${JAVA_EXE} -jar ${CUBE_JAR} -q .cube)
)

# Show MCU info
add_custom_target(info
    STM32_Programmer_CLI -c port=SWD
)

# Resets microcontroller
add_custom_target(reset
    COMMAND echo "Reseting device"
    COMMAND STM32_Programmer_CLI -c port=SWD -rst
)

# Clean all build files
add_custom_target(clean_all
    COMMAND echo "Cleaning all build files..."
    COMMAND rm -rf ./*
)

add_custom_target(vs_files
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/vs_files.cmake
)

# Clean cube generated files
add_custom_target(clean_cube
    COMMAND echo "Cleaning cube files..."
    COMMAND mv ../cube/*.ioc .
    COMMAND rm -rf ../cube/
    COMMAND mkdir ../cube/
    COMMAND mv *.ioc ../cube/
)

# Flash via st-link or jlink
function(thunder_flash_target TARGET)
    # Registra o comando make flash
    if("${TARGET}" STREQUAL "${PROJECT_NAME}")
        set(TARGET_SUFFIX "")
    else()
        set(TARGET_SUFFIX _${TARGET})
    endif()

    add_custom_target(flash${TARGET_SUFFIX}
        COMMAND echo "Flashing..."
        COMMAND STM32_Programmer_CLI -c port=SWD -w ${TARGET}.hex -v -rst
    )

    add_dependencies(flash${TARGET_SUFFIX} ${TARGET})

    file(WRITE .jlink-flash
        "device ${DEVICE}\n"
        "si SWD\n"
        "speed 4000\n"
        "connect\n"
        "r\n"
        "h\n"
        "loadfile ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.hex\n"
        "r\n"
        "g\n"
        "exit\n"
    )

    add_custom_target(jflash${TARGET_SUFFIX}
        COMMAND echo "Flashing ${PROJECT_NAME}.hex with J-Link"
        COMMAND ${JLINK_EXE} .jlink-flash
    )

    add_dependencies(jflash${TARGET_SUFFIX} ${TARGET})
endfunction()
