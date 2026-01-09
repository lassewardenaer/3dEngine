# Level library CMakeLists.txt
# This CMakeLists.txt builds the Level class as a static library

# Source files for level
set(LEVEL_SOURCES
    Level.cpp
)

# Create level static library
add_library(level STATIC ${LEVEL_SOURCES})

# Include directories for level
# PUBLIC means these directories are needed by consumers of this library
target_include_directories(level PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/level
)

# Link GLM if found via find_package
# PUBLIC because Level headers expose GLM types (glm::vec3)
if(glm_FOUND)
    target_link_libraries(level PUBLIC glm::glm)
endif()

