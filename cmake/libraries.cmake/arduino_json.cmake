if($ENV{ARDUINO_JSON_PATH})
    set(ARDUINO_JSON_PATH $ENV{ARDUINO_JSON_PATH})
else()
    set(ARDUINO_JSON_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArduinoJson/src)
endif()

if(NOT ARDUINO_JSON_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ARDUINO_JSON_PATH ?")
endif()

include_directories(${ARDUINO_JSON_PATH})
