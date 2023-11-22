if($ENV{MDNS_PATH})
    set(MDNS_PATH $ENV{MDNS_PATH})
else()
    set(MDNS_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArduinoMDNS/)
endif()

if(NOT MDNS_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set MDNS_PATH ?")
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

add_library(MDNS OBJECT
    ${MDNS_PATH}/MDNS.cpp
    ${MDNS_PATH}/MDNS.h
    ${MDNS_PATH}/utility/EthernetUtil.c
)

target_link_options(MDNS INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(MDNS INTERFACE cxx_std_11 c_std_11)

set_target_properties(MDNS PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(MDNS PUBLIC ArduinoFlags)
target_compile_features(MDNS PUBLIC cxx_std_11 c_std_11)
target_include_directories(MDNS PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${MDNS_PATH}
)

