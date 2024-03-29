cmake_minimum_required(VERSION 3.16)
project(HomeControllino LANGUAGES C CXX ASM)

include(cmake/lto.cmake)
include(cmake/arduino_core.cmake)
include(cmake/arduino_hex.cmake)
include(cmake/arduino_upload.cmake)
include(cmake/libraries.cmake/arduino_controllino.cmake)
include(cmake/libraries.cmake/ethernet.cmake)
include(cmake/libraries.cmake/simple_utility_library.cmake)
include(cmake/libraries.cmake/simple_webserver.cmake)
include(cmake/libraries.cmake/ntpclient.cmake)
include(cmake/libraries.cmake/arduino_json.cmake)
include(cmake/libraries.cmake/eeprom.cmake)
include(cmake/libraries.cmake/truerandom.cmake)
include(cmake/libraries.cmake/onewire.cmake)
include(cmake/libraries.cmake/dallastemperature.cmake)
include(cmake/libraries.cmake/arduino_rs485.cmake)
include(cmake/libraries.cmake/arduino_modbus_controllino.cmake)
include(cmake/libraries.cmake/spi.cmake)
include(cmake/libraries.cmake/mdns.cmake)
include(cmake/libraries.cmake/ethernet_webserver.cmake)
include(cmake/libraries.cmake/functional-vlpp.cmake)
include(cmake/libraries.cmake/arxcontainer.cmake)
include(cmake/libraries.cmake/acebutton.cmake)

add_executable(homecontrollino homecontrollino.cpp)
target_link_libraries(homecontrollino PUBLIC ArduinoCore)
target_link_libraries(homecontrollino PUBLIC ControllinoCore)
target_link_libraries(homecontrollino PUBLIC Ethernet)
target_link_libraries(homecontrollino PUBLIC SPI)
target_link_libraries(homecontrollino PUBLIC EEPROM)
target_link_libraries(homecontrollino PUBLIC MDNS)
target_link_libraries(homecontrollino PUBLIC EthernetWebserver)
target_link_libraries(homecontrollino PUBLIC TrueRandom)
target_link_libraries(homecontrollino PUBLIC ACEBUTTON)

target_compile_options(homecontrollino PUBLIC
    "-Wall"
    "-Wextra"
    "-pedantic"
    "-felide-constructors"
    "-std=c++0x"
)
arduino_avr_hex(homecontrollino)

add_executable(blink blink.cpp)
target_link_libraries(blink PUBLIC ArduinoCore)
target_link_libraries(blink PUBLIC ControllinoCore)
target_link_libraries(blink PUBLIC Ethernet)
target_link_libraries(blink PUBLIC SPI)
target_link_libraries(blink PUBLIC EEPROM)
target_link_libraries(blink PUBLIC MDNS)
target_link_libraries(blink PUBLIC EthernetWebserver)
target_link_libraries(blink PUBLIC TrueRandom)

target_compile_options(blink PUBLIC
    "-Wall"
    "-Wextra"
    "-pedantic"
)
arduino_avr_hex(blink)

set(ARDUINO_PORT "/dev/ttyACM1"
    CACHE STRING "The serial port for uploading using avrdude")
arduino_avr_upload(blink ${ARDUINO_PORT})
arduino_avr_upload(homecontrollino ${ARDUINO_PORT})
