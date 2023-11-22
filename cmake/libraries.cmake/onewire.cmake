if($ENV{ONEWIRE_PATH})
    set(ONEWIRE_PATH $ENV{ONEWIRE_PATH})
else()
    set(ONEWIRE_PATH ${CMAKE_HOME_DIRECTORY}/libraries/OneWire)
endif()

if(NOT ONEWIRE_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ONEWIRE_PATH ?")
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

add_library(OneWire OBJECT
    ${ONEWIRE_PATH}/OneWire.cpp
    ${ONEWIRE_PATH}/OneWire.h
    ${ONEWIRE_PATH}/util/OneWire_direct_gpio.h
    ${ONEWIRE_PATH}/util/OneWire_direct_regtype.h
)

target_link_options(OneWire INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(OneWire INTERFACE cxx_std_11 c_std_11)

target_link_libraries(OneWire PUBLIC ArduinoFlags)
target_compile_features(OneWire PUBLIC cxx_std_11 c_std_11)
target_include_directories(OneWire PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ONEWIRE_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

