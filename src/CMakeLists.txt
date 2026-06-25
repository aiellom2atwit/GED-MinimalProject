set(SOURCE_DIR "Source")

set(SOURCES
    main.cpp
    vec2.cpp
    gameengine.cpp
    entity.cpp
    entitymanager.cpp
    components.cpp
    action.cpp
    scene.cpp
    scenemenu.cpp
    sceneplay.cpp
    animation.cpp
    assets.cpp
    physics.cpp
)

# Add the executable target
add_executable(A3Student ${SOURCES})
set_target_properties(A3Student PROPERTIES OUTPUT_NAME prog.x)
target_include_directories(A3Student PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src)
target_include_directories(A3Student PRIVATE ${PROJECT_SOURCE_DIR}/external/)



target_link_libraries(A3Student imgui)
target_link_libraries(A3Student raylib)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()





