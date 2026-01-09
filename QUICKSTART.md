# Quick Start Guide

## Prerequisites Check

Before building, ensure you have:
- ✅ C++ compiler (GCC/Clang/MSVC)
- ✅ CMake 3.15+
- ✅ OpenGL 3.3+ support

## Step-by-Step Setup

### 1. Run the setup script (recommended)

```bash
./setup.sh
```

This will:
- Clone GLFW to `third_party/glfw`
- Check for GLAD and provide instructions if missing
- Attempt to install GLM

### 2. Manual Setup (if script doesn't work)

#### GLFW
```bash
cd third_party
git clone https://github.com/glfw/glfw.git
cd ..
```

#### GLAD
1. Visit https://glad.dav1d.de/
2. Configure:
   - Language: **C/C++**
   - Specification: **OpenGL**
   - API gl: **Version 3.3** or higher
   - Profile: **Core**
   - Generate a loader: **Yes**
3. Download and extract `include/` and `src/` to `third_party/glad/`

#### GLM

**macOS:**
```bash
brew install glm
```

**Linux:**
```bash
sudo apt-get install libglm-dev
```

**Windows:**
- Download from https://github.com/g-truc/glm/releases
- Extract to `third_party/glm/` or system include path

### 3. Build

```bash
mkdir build
cd build
cmake ..
make
```

### 4. Run

```bash
./DoomLikeGame
```

## Troubleshooting

### "GLFW not found"
- Make sure GLFW is cloned to `third_party/glfw/`
- Check that `third_party/glfw/CMakeLists.txt` exists

### "GLAD not found"
- Ensure `third_party/glad/include/glad/glad.h` exists
- Ensure `third_party/glad/src/glad.c` exists

### "GLM not found"
- Install via package manager (see above)
- Or download and place in `third_party/glm/`
- Update CMakeLists.txt if using custom path

### OpenGL errors
- Ensure your graphics drivers are up to date
- Check OpenGL version: `glxinfo | grep "OpenGL version"` (Linux) or use a tool like OpenGL Extensions Viewer

## Controls

- **W/A/S/D** - Move
- **Mouse** - Look around
- **Space** - Shoot
- **ESC** - Exit

Enjoy your Doom-like game!

