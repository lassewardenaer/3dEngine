# Renderer library CMakeLists.txt
# This CMakeLists.txt builds the renderer components as a static library

# Source files for renderer components
set(RENDERER_SOURCES
    Renderer.cpp
    WallRenderer.cpp
    FloorRenderer.cpp
    SkyRenderer.cpp
    BulletRenderer.cpp
)

# Create renderer static library
add_library(renderer STATIC ${RENDERER_SOURCES})

# Include directories for renderer
# PUBLIC means these directories are needed by consumers of this library
# Note: gameNodes and level include directories are also PUBLIC in their own CMakeLists.txt,
# but renderer needs them directly for its headers (e.g., Renderer.h includes Player.h)
target_include_directories(renderer PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/renderer
    ${CMAKE_SOURCE_DIR}/src/shader  # shader already has this as PUBLIC, but needed for renderer headers
    ${CMAKE_SOURCE_DIR}/src/gameNodes  # Needed for Player.h, Bullet.h includes
    ${CMAKE_SOURCE_DIR}/src/level  # Needed for Level.h includes
    ${CMAKE_SOURCE_DIR}/third_party/glad/include  # shader already has this as PUBLIC, but needed for renderer headers
)

# Link dependencies
# shader is PUBLIC because renderer exposes Shader class in its headers
# OpenGL is PRIVATE because it's only needed for building renderer
target_link_libraries(renderer 
    PRIVATE OpenGL::GL
    PUBLIC shader
)

# Link GLM if found via find_package
# PUBLIC because renderer headers may expose GLM types
if(glm_FOUND)
    target_link_libraries(renderer PUBLIC glm::glm)
endif()

