# Util library CMakeLists.txt
# This CMakeLists.txt builds utility classes (OsUtils, etc.) as a static library

# Source files for util
set(UTIL_SOURCES
    OsUtils.cpp
)

# Create util static library
add_library(util STATIC ${UTIL_SOURCES})

# Include directories for util
# PUBLIC means these directories are needed by consumers of this library
target_include_directories(util PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/util
)

# Link platform-specific frameworks (macOS) for OsUtils
if(APPLE)
    find_library(COREGRAPHICS_FRAMEWORK CoreGraphics REQUIRED)
    target_link_libraries(util PRIVATE ${COREGRAPHICS_FRAMEWORK})
endif()

# Link Windows libraries if on Windows
if(WIN32)
    # Windows-specific libraries can be added here if needed
endif()

# Link X11 libraries if on Linux
if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    if(X11_FOUND)
        target_link_libraries(util PRIVATE ${X11_LIBRARIES})
        target_include_directories(util PUBLIC ${X11_INCLUDE_DIR})
    endif()
endif()

