if($ENV{ARDUINO_MODBUS_CONTROLLINO_PATH})
    set(ARDUINO_MODBUS_CONTROLLINO_PATH $ENV{ARDUINO_MODBUS_CONTROLLINO_PATH})
else()
    set(ARDUINO_MODBUS_CONTROLLINO_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArduinoModbus_Controllino/src)
endif()

if(NOT ARDUINO_MODBUS_CONTROLLINO_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ARDUINO_MODBUS_CONTROLLINO_PATH ?")
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

add_library(ArduinoModbusControllino OBJECT
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusTCPClient.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusServer.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-rtu-private.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-private.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-rtu.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-tcp-private.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-data.c
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-tcp.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus.c
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-tcp.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-rtu.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/libmodbus/modbus-version.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusServer.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusRTUClient.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusTCPServer.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusRTUClient.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusRTUServer.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusTCPClient.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusClient.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusClient.cpp
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ArduinoModbus.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusTCPServer.h
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}/ModbusRTUServer.cpp
)

target_link_options(ArduinoModbusControllino INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(ArduinoModbusControllino INTERFACE cxx_std_11 c_std_11)

set_target_properties(ArduinoModbusControllino PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ArduinoModbusControllino PUBLIC ArduinoFlags)
target_compile_features(ArduinoModbusControllino PUBLIC cxx_std_11 c_std_11)
target_include_directories(ArduinoModbusControllino PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ARDUINO_RS485_PATH}
    ${ARDUINO_MODBUS_CONTROLLINO_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

