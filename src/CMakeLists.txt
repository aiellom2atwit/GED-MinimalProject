set(SOURCE_DIR "Source")

set(SOURCES
        main.cpp
        vec2.cpp
)

# Add the executable target
add_executable(Minimal ${SOURCES})
set_target_properties(Minimal PROPERTIES OUTPUT_NAME prog.x)

#Add external libraries to link, using public includes defined in the library
target_link_libraries(Minimal imgui)
target_link_libraries(Minimal raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()





