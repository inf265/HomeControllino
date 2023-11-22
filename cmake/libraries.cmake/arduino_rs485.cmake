if($ENV{ARDUINO_RS485_PATH})
    set(ARDUINO_RS485_PATH $ENV{ARDUINO_RS485_PATH})
else()
    set(ARDUINO_RS485_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArduinoRS485_Controllino/src)
endif()

if(NOT ARDUINO_RS485_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ARDUINO_RS485_PATH ?")
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

add_library(ArduinoRS485 OBJECT
    ${ARDUINO_RS485_PATH}/RS485.cpp
    ${ARDUINO_RS485_PATH}/ArduinoRS485.h
    ${ARDUINO_RS485_PATH}/RS485.h
)

target_link_options(ArduinoRS485 INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(ArduinoRS485 INTERFACE cxx_std_11 c_std_11)


set_target_properties(ArduinoRS485 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ArduinoRS485 PUBLIC ArduinoFlags)
target_compile_features(ArduinoRS485 PUBLIC cxx_std_11 c_std_11)
target_include_directories(ArduinoRS485 PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ONEWIRE_PATH}
    ${CONTROLLINO_PATH}
    ${ARDUINO_RS485_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

