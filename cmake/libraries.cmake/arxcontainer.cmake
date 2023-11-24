if($ENV{ARXCONTAINER_PATH})
    set(ARXCONTAINER_PATH $ENV{ARXCONTAINER_PATH})
else()
    set(ARXCONTAINER_PATH ${CMAKE_HOME_DIRECTORY}/libraries/ArxContainer)
endif()

if(NOT ARXCONTAINER_PATH)
    message(FATAL_ERROR "Arduino-specific variables are not set. \
                         Did you select the right toolchain file or set ARXCONTAINER_PATH ?")
endif()

include_directories(${ARXCONTAINER_PATH})