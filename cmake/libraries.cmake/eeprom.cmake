if($ENV{EEPROM_PATH})
    set(EEPROM_PATH $ENV{EEPROM_PATH})
else()
    set(EEPROM_PATH ${ARDUINO_AVR_PATH}/libraries/EEPROM/src)
endif()

if(NOT EEPROM_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set EEPROM_PATH ?")
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

add_library(EEPROM OBJECT
    ${EEPROM_PATH}/EEPROM.h
)

target_link_options(EEPROM INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(EEPROM INTERFACE cxx_std_11 c_std_11)

set_target_properties(EEPROM PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(EEPROM PUBLIC ArduinoFlags)
target_compile_features(EEPROM PUBLIC cxx_std_11 c_std_11)
target_include_directories(EEPROM PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${EEPROM_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

