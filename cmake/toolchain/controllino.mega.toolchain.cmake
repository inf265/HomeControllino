set(ARDUINO_BOARD "AVR_MEGA2560")
set(ARDUINO_MCU "atmega2560")
set(ARDUINO_F_CPU "16000000L")
set(ARDUINO_VARIANT "Controllino_mega")
set(ARDUINO_AVRDUDE_PROTOCOL "wiring")
set(ARDUINO_AVRDUDE_SPEED "115200")
set(ARDUINO_USB Off)

set(VARIANTS_HOME ${CMAKE_HOME_DIRECTORY}/libraries/CONTROLLINO_Library/Boards/avr/variants/)
include(${CMAKE_CURRENT_LIST_DIR}/avr.toolchain.cmake)

add_compile_definitions(CONTROLLINO_MEGA)
SET(CMAKE_CXX_FLAGS "-std=gnu++17")
