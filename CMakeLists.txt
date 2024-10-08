cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")

# glfw
find_package(glfw3 REQUIRED)

# glm
find_package(glm REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)


# Include directories
include_directories(include /usr/local/include /usr/include)

# Source files
file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.cpp" "src/*.hpp" "src/renderer/*.cpp" "src/renderer/*.hpp")

# Add the executable
add_executable(project_executable ${SOURCES})

# Link libraries
target_link_libraries(project_executable glfw glm::glm)

# Define asset directories
set(ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(ASSET_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/assets)

# Copy assets to build directory
add_custom_command(
    TARGET project_executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSET_DIR} ${ASSET_DEST_DIR}
)

# Define shaders directories
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Copy shaders to build directory
add_custom_command(
    TARGET project_executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_DIR} ${SHADERS_DEST_DIR}
)
