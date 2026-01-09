#!/bin/bash

# Setup script for Doom-Like Game dependencies

echo "Setting up Doom-Like Game dependencies..."

# Create third_party directory if it doesn't exist
mkdir -p third_party
cd third_party

# Clone GLFW if it doesn't exist
if [ ! -d "glfw" ]; then
    echo "Cloning GLFW..."
    git clone https://github.com/glfw/glfw.git
else
    echo "GLFW already exists, skipping..."
fi

# Check for GLAD
if [ ! -d "glad" ]; then
    echo ""
    echo "GLAD not found!"
    echo "Please download GLAD from https://glad.dav1d.de/"
    echo "Instructions:"
    echo "1. Go to https://glad.dav1d.de/"
    echo "2. Select: Language: C/C++, Specification: OpenGL, API gl: Version 3.3+, Profile: Core"
    echo "3. Click Generate and download the ZIP"
    echo "4. Extract the 'include' and 'src' folders to third_party/glad/"
    echo ""
else
    echo "GLAD found!"
fi

# Check for GLM
if [ ! -d "glm" ]; then
    echo ""
    echo "GLM not found in third_party/glm"
    echo "Installing GLM..."
    
    # Try to install via package manager
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        if command -v brew &> /dev/null; then
            echo "Installing GLM via Homebrew..."
            brew install glm
        else
            echo "Homebrew not found. Please install GLM manually or install Homebrew."
        fi
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        if command -v apt-get &> /dev/null; then
            echo "Installing GLM via apt-get..."
            sudo apt-get install -y libglm-dev
        elif command -v yum &> /dev/null; then
            echo "Installing GLM via yum..."
            sudo yum install -y glm-devel
        else
            echo "Please install GLM manually: sudo apt-get install libglm-dev"
        fi
    else
        echo "Please install GLM manually for your system."
    fi
else
    echo "GLM found in third_party/glm!"
fi

cd ..

echo ""
echo "Setup complete!"
echo ""
echo "Next steps:"
echo "1. Make sure GLAD is set up (see instructions above if needed)"
echo "2. Run: mkdir build && cd build && cmake .. && make"
echo "3. Run the game: ./DoomLikeGame"

