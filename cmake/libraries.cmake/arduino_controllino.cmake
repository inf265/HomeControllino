if($ENV{CONTROLLINO_PATH})
    set(CONTROLLINO_PATH $ENV{CONTROLLINO_PATH})
else()
    set(CONTROLLINO_PATH ${CMAKE_HOME_DIRECTORY}/libraries/CONTROLLINO_Library)
endif()

if(NOT CONTROLLINO_PATH)
    message(FATAL_ERROR "Controllino-specific variables are not set. \
                         Did you select the right toolchain file or set CONTROLLINO_PATH ?")
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



add_library(ControllinoCore OBJECT
    ${CONTROLLINO_PATH}/Controllino.cpp
)

target_link_options(ControllinoCore INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(ControllinoCore INTERFACE cxx_std_11 c_std_11)

target_link_libraries(ControllinoCore PUBLIC ArduinoFlags)
target_compile_features(ControllinoCore PUBLIC cxx_std_11 c_std_11)
target_include_directories(ControllinoCore PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ARDUINO_AVR_PATH}/variants/${ARDUINO_VARIANT}
    ${CONTROLLINO_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

