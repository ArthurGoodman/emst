cmake_minimum_required(VERSION 3.0)
project(emst)

################################################################################
### Settings
################################################################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

set(FLAGS "-Wall -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

################################################################################
### SFML
################################################################################

if(WIN32)
    set(SFML_ROOT ${CMAKE_SOURCE_DIR}/../SFML-2.4.2)
else()
    set(SFML_ROOT /usr/local/share/SFML)
endif()

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

add_executable(emst
    main.cpp
)

target_link_libraries(emst
    PRIVATE ${SFML_LIBRARIES}
    PRIVATE ${SFML_DEPENDENCIES}
)

target_include_directories(emst
    PRIVATE ${SFML_INCLUDE_DIR}
)

if(WIN32)
    file(GLOB SFML_DLLS ${SFML_ROOT}/bin/*.dll)

    add_custom_command(TARGET emst POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SFML_DLLS}
        $<TARGET_FILE_DIR:emst>
    )
endif()
