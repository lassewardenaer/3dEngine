# Shader library CMakeLists.txt
# This CMakeLists.txt builds the shader management system as a static library

# Source files for shader
set(SHADER_SOURCES
    Shader.cpp
)

# Create shader static library
add_library(shader STATIC ${SHADER_SOURCES})

# Include directories for shader
# PUBLIC means these directories are needed by consumers of this library
target_include_directories(shader PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/shader
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
)

# Link OpenGL (required for shader operations)
# PRIVATE means OpenGL is only needed for building shader, not by consumers
target_link_libraries(shader PRIVATE
    OpenGL::GL
)

# Link GLM if found via find_package
# PUBLIC because Shader headers may expose GLM types
if(glm_FOUND)
    target_link_libraries(shader PUBLIC glm::glm)
endif()

