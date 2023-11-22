if($ENV{SIMPLE_WEBSERVER_PATH})
    set(SIMPLE_WEBSERVER_PATH $ENV{SIMPLE_WEBSERVER_PATH})
else()
    set(SIMPLE_WEBSERVER_PATH ${CMAKE_HOME_DIRECTORY}/libraries/Simple-WebServer-Library/src)
endif()

if(NOT SIMPLE_WEBSERVER_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set SIMPLE_WEBSERVER_PATH ?")
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

add_library(SimpleWebserver OBJECT
    ${SIMPLE_WEBSERVER_PATH}/SimpleWebServer.cpp
    ${SIMPLE_WEBSERVER_PATH}/SimpleTask.cpp
)

target_link_options(SimpleWebserver INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(SimpleWebserver INTERFACE cxx_std_11 c_std_11)
#set_target_properties(SimpleWebserver PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(SimpleWebserver PUBLIC ArduinoFlags)
target_compile_features(SimpleWebserver PUBLIC cxx_std_11 c_std_11)
target_include_directories(SimpleWebserver PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${SIMPLE_UTILITY_LIB_PATH}
    ${SIMPLE_WEBSERVER_PATH}
    ${ETHERNET_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

