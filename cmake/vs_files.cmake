# Name: vs_files.cmake
# ThundeRatz Robotics Team
# 03/2023

# ##############################################################################
# # Auxiliary Functions
# ##############################################################################

# Function thunder_list_to_config_string
# Args: list_name, list1, list2, ...
# Returns: <list_name>
## If (list1 = dir1 dir2 dir3) and (list2 = dir4 dir5 dir6):
## than: (list_name = "dir1", "dir2", "dir3", "dir4", "dir5", "dir6")
function(thunder_list_to_config_string)
    list(GET ARGV 0 LIST_NAME)
    list(REMOVE_AT ARGV 0)

    string(REPLACE ";" "\", \"" RETVAL "${ARGV}")
    string(PREPEND RETVAL "\"")
    string(APPEND RETVAL "\"")

    set("${LIST_NAME}" ${RETVAL} PARENT_SCOPE)
endfunction()

###############################################################################
## VS Code files
###############################################################################

set(VS_LAUNCH_FILE ".vscode/launch.json")
set(VS_C_CPP_PROPERTIES_FILE ".vscode/c_cpp_properties.json")

thunder_list_to_config_string(INC_PATH ${C_INCLUDE_DIRECTORIES} ${TEST_INCLUDE_DIRECTORIES})
thunder_list_to_config_string(INC_FORCED ${FORCED_INCLUDE_DIRECTORIES})
thunder_list_to_config_string(COMPILE_DEF ${COMPILE_DEFINITIONS})

set(VS_LAUNCH
"{
    \"version\": \"0.2.0\",
    \"configurations\": [
        {
            \"type\": \"cortex-debug\",
            \"request\": \"launch\",
            \"servertype\": \"stutil\",
            \"cwd\": \"${CMAKE_CURRENT_SOURCE_DIR}\",
            \"executable\": \"build/${PROJECT_NAME}.elf\",
            \"name\": \"Cortex Debug (ST-Util)\",
            \"device\": \"${DEVICE}\",
            \"v1\": false
        },
        {
            \"type\": \"cortex-debug\",
            \"request\": \"launch\",
            \"servertype\": \"jlink\",
            \"cwd\": \"${CMAKE_CURRENT_SOURCE_DIR}\",
            \"executable\": \"build/${PROJECT_NAME}.elf\",
            \"name\": \"Cortex Debug (J-Link)\",
            \"device\": \"${DEVICE}\",
            \"interface\": \"swd\",
        }
    ]
}"
)

set(VS_CPP_PROPERTIES
"{
    \"configurations\": [
        {
            \"name\": \"STM32_TR\",

            \"includePath\": [
                ${INC_PATH}
            ],

			\"forcedInclude\": [
				${INC_FORCED}
			],

            \"defines\": [
                ${COMPILE_DEF}
            ],

            \"compilerPath\": \"$ENV{ARM_GCC_PATH}/arm-none-eabi-gcc\",
            \"cStandard\": \"c99\",
            \"cppStandard\": \"c++14\",
            \"intelliSenseMode\": \"clang-x64\"
        }
    ],
    \"version\": 4
}"
)

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${VS_LAUNCH_FILE} ${VS_LAUNCH})
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/${VS_C_CPP_PROPERTIES_FILE} ${VS_CPP_PROPERTIES})
