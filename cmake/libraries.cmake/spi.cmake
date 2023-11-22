if($ENV{SPI_PATH})
    set(SPI_PATH $ENV{SPI_PATH})
else()
    set(SPI_PATH ${ARDUINO_AVR_PATH}/libraries/SPI/src)
endif()

if(NOT SPI_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set SPI_PATH ?")
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

add_library(SPI OBJECT
    ${SPI_PATH}/SPI.cpp
    ${SPI_PATH}/SPI.h
)

target_link_options(SPI INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(SPI INTERFACE cxx_std_11 c_std_11)

set_target_properties(SPI PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SPI PUBLIC ArduinoFlags)
target_compile_features(SPI PUBLIC cxx_std_11 c_std_11)
target_include_directories(SPI PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${SPI_PATH}
)

