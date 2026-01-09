cmake_minimum_required(VERSION 3.15)
project(DoomLikeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLM
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    # Try to find GLM in common locations
    find_path(GLM_INCLUDE_DIR glm/glm.hpp
        PATHS
        /usr/include
        /usr/local/include
        /opt/homebrew/include
        ${CMAKE_SOURCE_DIR}/third_party/glm
    )
    if(GLM_INCLUDE_DIR)
        message(STATUS "Found GLM at: ${GLM_INCLUDE_DIR}")
        include_directories(${GLM_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "GLM not found! Please install GLM or place it in third_party/glm")
    endif()
else()
    message(STATUS "Found GLM via find_package")
endif()

# GLFW
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glfw/CMakeLists.txt")
    include_directories(third_party/glfw/include)
    add_subdirectory(third_party/glfw)
else()
    message(FATAL_ERROR "GLFW not found! Please clone it to third_party/glfw")
endif()

# GLAD
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glad/include/glad/glad.h")
    message(STATUS "Found GLAD headers")
else()
    message(FATAL_ERROR "GLAD not found! Please download GLAD and extract to third_party/glad/")
endif()

# Find GLAD source file
if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")
    set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/src/glad.c")
    set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/glad.c")
    message(STATUS "Using GLAD source from src/ directory")
else()
    message(FATAL_ERROR "GLAD source file (glad.c) not found! Please ensure it's in third_party/glad/src/ or src/")
endif()

# Add shader, renderer, gameNodes, level, and util as subdirectories (builds libraries)
add_subdirectory(src/shader)
add_subdirectory(src/renderer)
add_subdirectory(src/gameNodes)
add_subdirectory(src/level)
add_subdirectory(src/util)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Camera.cpp
    ${GLAD_SOURCE}
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
# shader is linked transitively through renderer (PUBLIC dependency)
# GLM is linked transitively through gameNodes, level, renderer (PUBLIC dependencies)
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    renderer  # Includes shader transitively
    gameNodes
    level
    util
)

# Note: CoreGraphics is linked through util library, no need to link here

# Include directories
# Most include directories are propagated through PUBLIC target_include_directories in libraries
# Only include directories not covered by libraries need to be specified here
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/glfw/include
)

# Enable testing
enable_testing()

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Test source files (without main.cpp and Game.cpp which require GLFW/OpenGL)
set(TEST_SOURCES
    src/Camera.cpp
    src/gameNodes/characters/Player.cpp
    src/gameNodes/weapons/Bullet.cpp
    src/gameNodes/characters/Enemy.cpp
    src/gameNodes/characters/Character.cpp
)

# Test executable
add_executable(DoomLikeGameTests
    tests/test_camera.cpp
    tests/test_player.cpp
    tests/test_level.cpp
    ${TEST_SOURCES}
)

# Link test libraries
# GLM is linked transitively through gameNodes, level (PUBLIC dependencies)
target_link_libraries(DoomLikeGameTests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    gameNodes
    level
    util
)

# Include directories for tests
# Most include directories are propagated through PUBLIC target_include_directories in libraries
target_include_directories(DoomLikeGameTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${GLM_INCLUDE_DIR}  # Only needed if GLM not found via find_package
)

# GLM is linked transitively through gameNodes and level (PUBLIC dependencies)

# Note: CoreGraphics is linked through util library, no need to link here

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(DoomLikeGameTests)

