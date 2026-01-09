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

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Camera.cpp
    src/Shader.cpp
    src/Level.cpp
    src/Player.cpp
    src/Renderer.cpp
    src/OsUtils.cpp
    ${GLAD_SOURCE}
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
)

# Link GLM if found via find_package
if(glm_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

# Link platform-specific frameworks (macOS)
if(APPLE)
    find_library(COREGRAPHICS_FRAMEWORK CoreGraphics REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_FRAMEWORK})
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
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
    src/Player.cpp
    src/Level.cpp
    src/OsUtils.cpp
)

# Test executable
add_executable(DoomLikeGameTests
    tests/test_camera.cpp
    tests/test_player.cpp
    tests/test_level.cpp
    ${TEST_SOURCES}
)

# Link test libraries
target_link_libraries(DoomLikeGameTests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
)

# Include directories for tests
target_include_directories(DoomLikeGameTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${GLM_INCLUDE_DIR}
)

# Link GLM if found via find_package
if(glm_FOUND)
    target_link_libraries(DoomLikeGameTests PRIVATE glm::glm)
endif()

# Link platform-specific frameworks (macOS) for OsUtils tests
if(APPLE)
    target_link_libraries(DoomLikeGameTests PRIVATE ${COREGRAPHICS_FRAMEWORK})
endif()

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(DoomLikeGameTests)

