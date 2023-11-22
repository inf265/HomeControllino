if($ENV{ETHERNET_PATH})
    set(ETHERNET_PATH $ENV{ETHERNET_PATH})
else()
    set(ETHERNET_PATH ${ARDUINO_LIB_PATH}/Ethernet/src)
endif()

if(NOT ETHERNET_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ETHERNET_PATH ?")
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

add_library(Ethernet OBJECT
    ${ETHERNET_PATH}/Dhcp.cpp
    ${ETHERNET_PATH}/Dns.cpp
    ${ETHERNET_PATH}/Ethernet.cpp
    ${ETHERNET_PATH}/EthernetClient.cpp
    ${ETHERNET_PATH}/EthernetServer.cpp
    ${ETHERNET_PATH}/EthernetUdp.cpp
    ${ETHERNET_PATH}/socket.cpp
    ${ETHERNET_PATH}/utility/w5100.cpp
)

target_link_options(Ethernet INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(Ethernet INTERFACE cxx_std_11 c_std_11)
#set_target_properties(Ethernet PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Ethernet PUBLIC ArduinoFlags)
target_compile_features(Ethernet PUBLIC cxx_std_11 c_std_11)
target_include_directories(Ethernet PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ETHERNET_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

