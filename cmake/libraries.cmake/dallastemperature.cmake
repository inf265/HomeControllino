if($ENV{DALLASTEMPERATURE_PATH})
    set(DALLASTEMPERATURE_PATH $ENV{DALLASTEMPERATURE_PATH})
else()
    set(DALLASTEMPERATURE_PATH ${CMAKE_HOME_DIRECTORY}/libraries/DallasTemperature)
endif()

if(NOT DALLASTEMPERATURE_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set DALLASTEMPERATURE_PATH ?")
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

add_library(DallasTemperature OBJECT
    ${DALLASTEMPERATURE_PATH}/DallasTemperature.cpp
    ${DALLASTEMPERATURE_PATH}/DallasTemperature.h
)

target_link_options(DallasTemperature INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(DallasTemperature INTERFACE cxx_std_11 c_std_11)


target_link_libraries(DallasTemperature PUBLIC ArduinoFlags)
target_compile_features(DallasTemperature PUBLIC cxx_std_11 c_std_11)
target_include_directories(DallasTemperature PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ONEWIRE_PATH}
    ${DALLASTEMPERATURE_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

