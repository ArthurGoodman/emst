file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(emst ${SOURCES})

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
