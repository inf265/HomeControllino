if($ENV{ACEBUTTON_PATH})
    set(ACEBUTTON_PATH $ENV{ACEBUTTON_PATH})
else()
    set(ACEBUTTON_PATH ${CMAKE_HOME_DIRECTORY}/libraries/AceButton/src)
endif()

if(NOT ACEBUTTON_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ACEBUTTON_PATH ?")
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

add_library(ACEBUTTON OBJECT
    ${ACEBUTTON_PATH}/ace_button/LadderButtonConfig.cpp
    ${ACEBUTTON_PATH}/ace_button/EncodedButtonConfig.cpp
    ${ACEBUTTON_PATH}/ace_button/ButtonConfig.cpp
    ${ACEBUTTON_PATH}/ace_button/testing/EventTracker.cpp
    ${ACEBUTTON_PATH}/ace_button/AceButton.cpp
)

target_link_options(ACEBUTTON INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(ACEBUTTON INTERFACE cxx_std_11 c_std_11)

set_target_properties(ACEBUTTON PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(ACEBUTTON PUBLIC ArduinoFlags)
target_compile_features(ACEBUTTON PUBLIC cxx_std_11 c_std_11)
target_include_directories(ACEBUTTON PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ACEBUTTON_PATH}
)

