file(GLOB MAIN_SRCS ./*.cpp ./**/*.cpp)

add_library(Main ${MAIN_SRCS})

target_compile_definitions(Main PRIVATE ${ARDUINO_COMPILE_DEFINITIONS})

target_compile_options(Main PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-std=gnu11>
    $<$<COMPILE_LANGUAGE:CXX>:-Wvla>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++14>

    ${ARDUINO_COMPILE_OPTIONS}
)

set(MAIN_INCLUDE_DIRS
    ${ARDUINO_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Append BLE library include back to ARDUINO_INCLUDE_DIRS at parent scope
set(ARDUINO_INCLUDE_DIRS
    "${MAIN_INCLUDE_DIRS}"
    PARENT_SCOPE
)

target_include_directories(Main PRIVATE
    ${MAIN_INCLUDE_DIRS}
)