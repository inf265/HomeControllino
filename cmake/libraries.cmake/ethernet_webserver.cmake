if($ENV{ETHERNET_WEBSERVER_PATH})
    set(ETHERNET_WEBSERVER_PATH $ENV{ETHERNET_WEBSERVER_PATH})
else()
    set(ETHERNET_WEBSERVER_PATH ${CMAKE_HOME_DIRECTORY}/libraries/EthernetWebServer/src)
endif()

if(NOT ETHERNET_WEBSERVER_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ETHERNET_WEBSERVER_PATH ?")
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

add_library(EthernetWebserver OBJECT
    ${ETHERNET_WEBSERVER_PATH}/EthernetWebServer.hpp
    ${ETHERNET_WEBSERVER_PATH}/Ethernet_HTTPClient/Ethernet_HttpClient.cpp
    ${ETHERNET_WEBSERVER_PATH}/Ethernet_HTTPClient/Ethernet_URLEncoder.cpp
    ${ETHERNET_WEBSERVER_PATH}/Ethernet_HTTPClient/Ethernet_WebSocketClient.cpp
    ${ETHERNET_WEBSERVER_PATH}/detail/esp_detail/mimetable.cpp
    ${ETHERNET_WEBSERVER_PATH}/libb64/base64.cpp
    ${ETHERNET_WEBSERVER_PATH}/libb64/cencode.c
    ${ETHERNET_WEBSERVER_PATH}/libb64/cdecode.c
)

target_link_options(EthernetWebserver INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(EthernetWebserver INTERFACE cxx_std_11 c_std_11)
#set_target_properties(SimpleWebserver PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(EthernetWebserver PUBLIC ArduinoFlags)
target_compile_features(EthernetWebserver PUBLIC cxx_std_11 c_std_11)
target_include_directories(EthernetWebserver PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ETHERNET_WEBSERVER_PATH}
    ${ETHERNET_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

