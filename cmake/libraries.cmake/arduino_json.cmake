if($ENV{ARDUINO_JSON_PATH})
    set(ARDUINO_JSON_PATH $ENV{ARDUINO_JSON_PATH})
else()
    set(ARDUINO_JSON_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArduinoJson-5.x/src)
endif()

if(NOT ARDUINO_JSON_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ARDUINO_JSON_PATH ?")
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

add_library(ArduinoJson OBJECT
    ${ARDUINO_JSON_PATH}/ArduinoJson.h
    ${ARDUINO_JSON_PATH}/ArduinoJson.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantBase.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StaticJsonBuffer.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/DynamicJsonBuffer.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/StdString.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/CharPointer.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/StringTraits.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/StdStream.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/FlashString.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/StringTraits/ArduinoStream.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantComparisons.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonObjectImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonArrayImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonBufferBase.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Deserialization
    ${ARDUINO_JSON_PATH}/ArduinoJson/Deserialization/Comments.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Deserialization/JsonParser.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Deserialization/JsonParserImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Deserialization/StringWriter.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonBuffer.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/Prettyfier.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/JsonSerializer.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/StreamPrintAdapter.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/StaticStringBuilder.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/DummyPrint.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/JsonPrintable.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/FloatParts.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/JsonWriter.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/IndentedPrint.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/JsonSerializerImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Serialization/DynamicStringBuilder.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonObjectSubscript.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonArray.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Configuration.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/ReferenceType.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonVariantContent.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonVariantType.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonFloat.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/NonCopyable.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonVariantDefault.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/List.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/ValueSaver.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/ListConstIterator.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonBufferAllocated.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonInteger.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/JsonVariantAs.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/ListNode.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/Encoding.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Data/ListIterator.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantOr.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsUnsignedIntegral.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/RemoveConst.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsSame.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/FloatTraits.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsArray.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsChar.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsConst.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/RemoveReference.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsVariant.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsIntegral.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsSignedIntegral.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsBaseOf.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/IsFloatingPoint.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/TypeTraits/EnableIf.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/version.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantCasts.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonArraySubscript.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/compatibility.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/RawJson.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonObject.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonPair.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/parseFloat.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/alias_cast.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/parseInteger.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/ctype.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/isInteger.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/isFloat.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/attributes.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/Polyfills/math.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonBufferImpl.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariantSubscripts.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson/JsonVariant.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson.hpp
    ${ARDUINO_JSON_PATH}/ArduinoJson.h
)

target_link_options(ArduinoJson INTERFACE
    "-mmcu=${ARDUINO_MCU}"
    "-fuse-linker-plugin"
    "LINKER:--gc-sections"
)
target_compile_features(ArduinoJson INTERFACE cxx_std_11 c_std_11)


set_target_properties(ArduinoJson PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ArduinoJson PUBLIC ArduinoFlags)
target_compile_features(ArduinoJson PUBLIC cxx_std_11 c_std_11)
target_include_directories(ArduinoJson PUBLIC
    ${ARDUINO_CORE_PATH}
    ${VARIANTS_HOME}/${ARDUINO_VARIANT}
    ${ARDUINO_JSON_PATH}
    ${ARDUINO_AVR_PATH}/libraries/SPI/src
)

