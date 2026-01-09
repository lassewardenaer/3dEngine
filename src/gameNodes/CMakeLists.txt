# GameNodes library CMakeLists.txt
# This CMakeLists.txt builds the game node components (Character, Player, Bullet, etc.) as a static library

# Source files for gameNodes
set(GAMENODES_SOURCES
    characters/Character.cpp
    characters/Player.cpp
    characters/Enemy.cpp
    weapons/Bullet.cpp
)

# Create gameNodes static library
add_library(gameNodes STATIC ${GAMENODES_SOURCES})

# Include directories for gameNodes
# PUBLIC means these directories are needed by consumers of this library
target_include_directories(gameNodes PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/gameNodes
    ${CMAKE_SOURCE_DIR}/src/gameNodes/characters
    ${CMAKE_SOURCE_DIR}/src/gameNodes/weapons
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
)

# Link GLM if found via find_package
# PUBLIC because gameNodes headers expose GLM types (glm::vec3)
if(glm_FOUND)
    target_link_libraries(gameNodes PUBLIC glm::glm)
endif()

