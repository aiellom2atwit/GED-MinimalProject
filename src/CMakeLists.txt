set(SOURCE_DIR "Source")

set(SOURCES
        main.cpp
        vec2.cpp
)

# Add the executable target
add_executable(Engine ${SOURCES})
set_target_properties(Engine PROPERTIES OUTPUT_NAME prog.x)
target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src)
target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/external/)



target_link_libraries(Engine imgui)
target_link_libraries(Engine raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()





