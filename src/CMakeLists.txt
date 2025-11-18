set(ENGINE_SRC
    Engine/Main.cpp
    Engine/Engine.cpp
    Platform/Window.cpp
)

add_executable(MyEngine ${ENGINE_SRC})

target_include_directories(MyEngine PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Link GLFW and OpenGL
target_link_libraries(MyEngine glfw)

# Link OpenGL framework on macOS
if(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    target_link_libraries(MyEngine ${OPENGL_LIBRARY})
endif()
