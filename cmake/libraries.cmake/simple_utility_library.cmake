if($ENV{SIMPLE_UTILITY_LIB_PATH})
    set(SIMPLE_UTILITY_LIB_PATH $ENV{SIMPLE_UTILITY_LIB_PATH})
else()
    set(SIMPLE_UTILITY_LIB_PATH ${CMAKE_HOME_DIRECTORY}/libraries/Simple-Utility-Library/src)
endif()

if(NOT SIMPLE_UTILITY_LIB_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set SIMPLE_UTILITY_LIB_PATH ?")
endif()

set(SRC_PATH ${CMAKE_HOME_DIRECTORY}/src)

add_compile_definitions(
    "ARDUINO_BOARD=AVR_MEGA2560"
    "ARDUINO_MCU=atmega2560"
    "F_CPU=${ARDUINO_F_CPU}"
    "ARDUINO=${ARDUINO_VERSION}"
    "ARDUINO_${ARDUINO_BOARD}"
    "ARDUINO_ARCH_AVR"
)

add_compile_options(
    "-fno-exceptions"
    "-ffunction-sections"
    "-fdata-sections"
    "$<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>"
    "-mmcu=${ARDUINO_MCU}"
)

add_library(SimpleUtilityLibrary OBJECT
    ${SIMPLE_UTILITY_LIB_PATH}/SimpleBuffer.cpp
    ${SIMPLE_UTILITY_LIB_PATH}/SimpleHttp.cpp
    ${SIMPLE_UTILITY_LIB_PATH}/SimpleTimer.cpp
    ${SIMPLE_UTILITY_LIB_PATH}/SimpleUtils.cpp
)

target_link_options(SimpleUtilityLibrary INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(SimpleUtilityLibrary INTERFACE cxx_std_11 c_std_11)

target_link_libraries(SimpleUtilityLibrary PUBLIC ArduinoFlags)
target_compile_features(SimpleUtilityLibrary PUBLIC cxx_std_11 c_std_11)
target_include_directories(SimpleUtilityLibrary PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${SIMPLE_UTILITY_LIB_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

