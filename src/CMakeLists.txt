file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(emst ${SOURCES})

target_link_libraries(emst
    PRIVATE ${SFML_LIBRARIES}
    PRIVATE ${SFML_DEPENDENCIES}
)
target_include_directories(emst
    PRIVATE ${SFML_INCLUDE_DIR}
)
