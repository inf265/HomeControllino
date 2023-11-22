if($ENV{NTPCLIENT_PATH})
    set(NTPCLIENT_PATH $ENV{NTPCLIENT_PATH})
else()
    set(NTPCLIENT_PATH ${CMAKE_HOME_DIRECTORY}/libraries/NTPClient)
endif()

if(NOT NTPCLIENT_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set NTPCLIENT_PATH ?")
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

add_library(NtpClient OBJECT
    ${NTPCLIENT_PATH}/NTPClient.cpp
    ${NTPCLIENT_PATH}/NTPClient.h
)

target_link_options(NtpClient INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(NtpClient INTERFACE cxx_std_11 c_std_11)

target_link_libraries(NtpClient PUBLIC ArduinoFlags)
target_compile_features(NtpClient PUBLIC cxx_std_11 c_std_11)
target_include_directories(NtpClient PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${NTPCLIENT_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

