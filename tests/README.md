# Unit Tests

This directory contains unit tests for the Doom-Like Game project using Google Test.

## Running Tests

### Build and Run All Tests

```bash
cd build
cmake ..
make DoomLikeGameTests
./DoomLikeGameTests
```

### Run Tests with CTest

```bash
cd build
ctest
```

### Run Specific Test Suite

```bash
cd build
./DoomLikeGameTests --gtest_filter=CameraTest.*
./DoomLikeGameTests --gtest_filter=PlayerTest.*
./DoomLikeGameTests --gtest_filter=LevelTest.*
```

## Test Coverage

### Camera Tests (10 tests)
- Default constructor
- Position setting
- Yaw setting and vector updates
- Pitch clamping
- View matrix generation
- Projection matrix generation
- Vector normalization
- Vector orthogonality

### Player Tests (12 tests)
- Default constructor
- Position setting
- Movement (forward, backward, left, right)
- Position update and previous position tracking
- Mouse movement processing
- Pitch clamping in mouse movement
- Shooting mechanics
- Shoot cooldown
- Movement speed

### Level Tests (12 tests)
- Initialization
- Wall creation
- Floor creation
- Collision detection (various scenarios)
- Floor vertex structure
- Floor indices validity
- Shutdown and cleanup

## Test Structure

Tests are organized by component:
- `test_camera.cpp` - Camera system tests
- `test_player.cpp` - Player controller tests
- `test_level.cpp` - Level and collision detection tests

## Adding New Tests

To add new tests:

1. Create a new test file in the `tests/` directory (e.g., `test_newcomponent.cpp`)
2. Include the necessary headers and Google Test
3. Add the test file to `CMakeLists.txt` in the `add_executable` section
4. Rebuild and run tests

Example test structure:

```cpp
#include <gtest/gtest.h>
#include "YourComponent.h"

class YourComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        component = YourComponent();
    }
    
    YourComponent component;
};

TEST_F(YourComponentTest, YourTestName) {
    // Test code here
    EXPECT_EQ(expected, actual);
}
```

