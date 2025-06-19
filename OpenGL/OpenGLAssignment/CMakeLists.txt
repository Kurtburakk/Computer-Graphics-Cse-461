cmake_minimum_required(VERSION 3.10)
project(OpenGLAssignment)

set(CMAKE_CXX_STANDARD 17)

# Add your own include directory and src
include_directories(
    include
    /opt/homebrew/include          # For Apple Silicon Homebrew installs
    /usr/local/include             # For Intel Mac Homebrew installs
)

link_directories(
    /opt/homebrew/lib
    /usr/local/lib
)

find_package(assimp REQUIRED)

# Sources
set(SOURCES
    src/main.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Object.cpp
    src/World.cpp
    src/glad.c
    src/stb_image.cpp
    src/Model.cpp
    src/Mesh.cpp
    src/Input.cpp
)

add_executable(OpenGLAssignment ${SOURCES})

# Include Assimp's headers
target_include_directories(OpenGLAssignment PRIVATE ${ASSIMP_INCLUDE_DIRS})

# Link GLFW, OpenGL (use OpenGL framework for Mac), and Assimp
target_link_libraries(OpenGLAssignment
    glfw
    "-framework OpenGL"
    assimp
)
