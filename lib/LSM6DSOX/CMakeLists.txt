
set(LIBRARY_NAME LSM6DSOX)

file(GLOB LIB_SRCS src/*.cpp src/**/*.cpp src/**/*.c src/*.c)

add_library(${LIBRARY_NAME} ${LIB_SRCS})

target_compile_definitions(${LIBRARY_NAME} PRIVATE ${ARDUINO_COMPILE_DEFINITIONS})

target_compile_options(${LIBRARY_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    $<$<COMPILE_LANGUAGE:CXX>:-Wvla>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++14>

    ${ARDUINO_COMPILE_OPTIONS}
)

set(LIB_INCLUDE_DIRS
    ${ARDUINO_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Append BLE library include back to ARDUINO_INCLUDE_DIRS at parent scope
set(ARDUINO_INCLUDE_DIRS
    "${LIB_INCLUDE_DIRS}"
    PARENT_SCOPE
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    ${LIB_INCLUDE_DIRS}
)

