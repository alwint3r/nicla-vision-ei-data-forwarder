cmake_minimum_required(VERSION 3.20)

project(ArduinoCmake C CXX ASM)

include(cmake/nicla_vision.cmake)
include(cmake/include_library.cmake)

arduino_include_library(Wire)
arduino_include_library(SocketWrapper)
arduino_include_library(WiFi)


add_subdirectory(lib/ArduinoBLE)
add_subdirectory(src)

add_library(core STATIC
    ${ARDUINO_CORE_C_SRCS}
    ${ARDUINO_CORE_SRCS}
    ${ARDUINO_API_SRCS}
    ${ARDUINO_API_C_SRCS}
)

add_executable(firmware.elf
    ${ARDUINO_TARGET_SRCS}
)

target_include_directories(firmware.elf PRIVATE
    ${ARDUINO_INCLUDE_DIRS}
)

target_include_directories(core PRIVATE
    ${ARDUINO_INCLUDE_DIRS}
)

target_link_libraries(firmware.elf
    ${ARDUINO_COMMON_LINKFLAGS}

    -T${LINKER_SCRIPT}

    --specs=nosys.specs

    -Wl,--whole-archive
    core
    Wire
    ArduinoBLE
    SocketWrapper
    WiFi
    Main

    ${ARDUINO_CORE_DIR}/variants/NICLA_VISION/libs/libmbed.a

    ${ARDUINO_CLOSING_LINKFLAGS}
)

target_compile_definitions(firmware.elf PRIVATE
    ${ARDUINO_COMPILE_DEFINITIONS}
)

target_compile_definitions(core PRIVATE
    ${ARDUINO_COMPILE_DEFINITIONS}
)


target_compile_options(firmware.elf PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    $<$<COMPILE_LANGUAGE:CXX>:-Wvla>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++14>

    ${ARDUINO_COMPILE_OPTIONS}
)

target_compile_options(core PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    $<$<COMPILE_LANGUAGE:CXX>:-Wvla>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++14>

    ${ARDUINO_COMPILE_OPTIONS}
)

add_custom_command(OUTPUT firmware.bin DEPENDS firmware.elf COMMAND ${CMAKE_OBJCOPY} -Obinary firmware.elf firmware.bin)

add_custom_command(OUTPUT firmware.hex DEPENDS firmware.elf COMMAND ${CMAKE_OBJCOPY} -Oihex firmware.elf firmware.hex)

add_custom_target(bin
    DEPENDS firmware.bin
)

add_custom_target(hex
    DEPENDS firmware.hex
)

add_custom_command(TARGET firmware.elf POST_BUILD COMMAND ${CMAKE_SIZE} --format=berkeley firmware.elf)

