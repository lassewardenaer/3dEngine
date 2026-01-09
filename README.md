# Doom-Like Game - OpenGL C++ Project

A first-person shooter game built with C++, OpenGL, GLFW, and GLM.

## Features

- First-person camera with mouse look
- WASD movement controls
- 3D level with walls and floor
- Phong lighting model
- Collision detection
- Shooting mechanics (spacebar)

## Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.15 or higher
- OpenGL 3.3 or higher
- GLM (OpenGL Mathematics library)

## Dependencies

This project requires:
- **GLFW** - Window and input management
- **GLAD** - OpenGL loader
- **GLM** - Mathematics library

## Setup Instructions

### 1. Clone or download GLFW

```bash
cd third_party
git clone https://github.com/glfw/glfw.git
```

### 2. Setup GLAD

1. Go to https://glad.dav1d.de/
2. Select:
   - Language: C/C++
   - Specification: OpenGL
   - API gl: Version 3.3 or higher
   - Profile: Core
   - Generate a loader: Yes
3. Click "Generate"
4. Download the generated ZIP file
5. Extract the `include` and `src` folders to `third_party/glad/`

### 3. Install GLM

#### macOS (using Homebrew):
```bash
brew install glm
```

#### Linux (Ubuntu/Debian):
```bash
sudo apt-get install libglm-dev
```

#### Windows:
Download from https://github.com/g-truc/glm/releases and extract to a location, then update CMakeLists.txt to point to it.

### 4. Build the Project

```bash
mkdir build
cd build
cmake ..
make
```

### 5. Run the Game

```bash
./DoomLikeGame
```

## Controls

- **W, A, S, D** - Move forward, left, backward, right
- **Mouse** - Look around
- **Space** - Shoot
- **ESC** - Exit game

## Project Structure

```
openGLproject/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp/h         # Main game loop
│   ├── Camera.cpp/h       # Camera system
│   ├── Player.cpp/h       # Player controller
│   ├── Level.cpp/h        # Level/map system
│   ├── Renderer.cpp/h     # Rendering system
│   └── Shader.cpp/h       # Shader management
└── third_party/           # External dependencies
    ├── glfw/              # GLFW library
    └── glad/              # GLAD loader
        ├── include/
        └── src/
```

## Future Enhancements

- Texture mapping for walls and floor
- Enemy AI and spawning
- Weapon models and animations
- Sound effects and music
- Health and ammo systems
- More complex level geometry
- Particle effects

## License

This is a learning project. Feel free to use and modify as needed.

