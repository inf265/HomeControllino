if($ENV{FUNCTIONAL_VLPP_PATH})
    set(FUNCTIONAL_VLPP_PATH $ENV{FUNCTIONAL_VLPP_PATH})
else()
    set(FUNCTIONAL_VLPP_PATH ${CMAKE_HOME_DIRECTORY}/libraries/functional-vlpp/src)
endif()

if(NOT FUNCTIONAL_VLPP_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set FUNCTIONAL_VLPP_PATH ?")
endif()

include_directories(${FUNCTIONAL_VLPP_PATH})