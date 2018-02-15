cmake_minimum_required(VERSION 3.0)
project(emst)

################################################################################
### Settings
################################################################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

set(FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

################################################################################
### SFML
################################################################################

set(SFML_ROOT /usr/local/share/SFML)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${SFML_ROOT}/cmake/Modules)

find_package(SFML 2
    COMPONENTS
        graphics
        window
        system
    REQUIRED
)

################################################################################
### Targets
################################################################################

add_subdirectory(src)
