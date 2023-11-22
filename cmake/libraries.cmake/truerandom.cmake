if($ENV{TRUERANDOM_PATH})
    set(TRUERANDOM_PATH $ENV{TRUERANDOM_PATH})
else()
    set(TRUERANDOM_PATH ${CMAKE_HOME_DIRECTORY}/libraries/TrueRandom)
endif()

if(NOT TRUERANDOM_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set TRUERANDOM_PATH ?")
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

add_library(TrueRandom OBJECT
    ${TRUERANDOM_PATH}/TrueRandom.cpp
    ${TRUERANDOM_PATH}/TrueRandom.h
)

target_link_options(TrueRandom INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(TrueRandom INTERFACE cxx_std_11 c_std_11)

target_link_libraries(TrueRandom PUBLIC ArduinoFlags)
target_compile_features(TrueRandom PUBLIC cxx_std_11 c_std_11)
target_include_directories(TrueRandom PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${TRUERANDOM_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

