#include <gtest/gtest.h>
#include "Level.h"
#include <glm/gtc/epsilon.hpp>

class LevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        level = Level();
        level.Initialize();
    }
    
    void TearDown() override {
        level.Shutdown();
    }
    
    Level level;
    const float EPSILON = 0.001f;
};

// Test initialization
TEST_F(LevelTest, Initialize) {
    EXPECT_TRUE(level.Initialize());
    
    // Should have walls
    const auto& walls = level.GetWalls();
    EXPECT_GT(walls.size(), 0);
    
    // Should have floor vertices
    const auto& floorVertices = level.GetFloorVertices();
    EXPECT_GT(floorVertices.size(), 0);
    
    // Should have floor indices
    const auto& floorIndices = level.GetFloorIndices();
    EXPECT_GT(floorIndices.size(), 0);
}

// Test wall creation
TEST_F(LevelTest, WallsCreated) {
    const auto& walls = level.GetWalls();
    
    // Should have at least outer walls (4) and inner walls
    EXPECT_GE(walls.size(), 4);
    
    // Check that walls have valid data
    for (const auto& wall : walls) {
        EXPECT_GT(wall.height, 0.0f);
        float wallLength = glm::length(wall.end - wall.start);
        EXPECT_GT(wallLength, 0.0f);
    }
}

// Test floor creation
TEST_F(LevelTest, FloorCreated) {
    const auto& floorVertices = level.GetFloorVertices();
    const auto& floorIndices = level.GetFloorIndices();
    
    // Floor should have 4 vertices (position + normal = 6 floats per vertex)
    // 4 vertices * 6 floats = 24 floats
    EXPECT_EQ(24, floorVertices.size());
    
    // Floor should have 2 triangles = 6 indices
    EXPECT_EQ(6, floorIndices.size());
}

// Test collision detection - no collision in center
TEST_F(LevelTest, NoCollisionInCenter) {
    glm::vec3 centerPos(0.0f, 0.5f, 0.0f);
    float radius = 0.3f;
    
    bool collision = level.CheckCollision(centerPos, radius);
    EXPECT_FALSE(collision);
}

// Test collision detection - collision with boundary
TEST_F(LevelTest, CollisionWithBoundary) {
    // Position at the boundary (level size is 20.0f, so boundary is at ±20.0)
    // With radius 0.5f, collision occurs when position.x > 20.0 - 0.5 = 19.5
    glm::vec3 boundaryPos(19.6f, 0.5f, 0.0f);
    float radius = 0.5f;
    
    bool collision = level.CheckCollision(boundaryPos, radius);
    EXPECT_TRUE(collision);
}

// Test collision detection - collision with wall
TEST_F(LevelTest, CollisionWithWall) {
    // Position very close to a wall (inner wall at x = -5.0f, runs from z=-5 to z=5)
    // Position at x=-4.4 with radius 0.5 should collide with wall at x=-5.0
    // Distance from wall: 5.0 - 4.4 = 0.6, but with radius 0.5, we need to be closer
    // Actually, distance is 0.6 which is > 0.5, so let's use x=-4.3 (distance 0.7) with larger radius
    // Or better: use x=-4.4 with radius 0.6
    glm::vec3 wallPos(-4.4f, 0.5f, 0.0f);
    float radius = 0.6f;
    
    bool collision = level.CheckCollision(wallPos, radius);
    EXPECT_TRUE(collision);
}

// Test collision detection - no collision when far from walls
TEST_F(LevelTest, NoCollisionFarFromWalls) {
    glm::vec3 safePos(2.0f, 0.5f, 2.0f);
    float radius = 0.3f;
    
    bool collision = level.CheckCollision(safePos, radius);
    EXPECT_FALSE(collision);
}

// Test collision detection - different radius sizes
TEST_F(LevelTest, CollisionWithDifferentRadius) {
    glm::vec3 pos(18.0f, 0.5f, 0.0f);
    
    // Small radius - no collision
    bool collisionSmall = level.CheckCollision(pos, 0.1f);
    EXPECT_FALSE(collisionSmall);
    
    // Large radius - collision
    bool collisionLarge = level.CheckCollision(pos, 2.5f);
    EXPECT_TRUE(collisionLarge);
}

// Test collision detection - negative boundary
TEST_F(LevelTest, CollisionWithNegativeBoundary) {
    // Position at negative boundary (level size is 20.0f, so boundary is at -20.0)
    // With radius 0.5f, collision occurs when position.x < -20.0 + 0.5 = -19.5
    glm::vec3 negBoundaryPos(-19.6f, 0.5f, 0.0f);
    float radius = 0.5f;
    
    bool collision = level.CheckCollision(negBoundaryPos, radius);
    EXPECT_TRUE(collision);
}

// Test floor vertex structure
TEST_F(LevelTest, FloorVertexStructure) {
    const auto& floorVertices = level.GetFloorVertices();
    
    // Each vertex should have 6 floats (3 for position, 3 for normal)
    EXPECT_EQ(0, floorVertices.size() % 6);
    
    // Check that normals are pointing up (0, 1, 0)
    for (size_t i = 3; i < floorVertices.size(); i += 6) {
        EXPECT_NEAR(0.0f, floorVertices[i], EPSILON);     // normal.x
        EXPECT_NEAR(1.0f, floorVertices[i + 1], EPSILON); // normal.y
        EXPECT_NEAR(0.0f, floorVertices[i + 2], EPSILON); // normal.z
    }
}

// Test floor indices validity
TEST_F(LevelTest, FloorIndicesValid) {
    const auto& floorIndices = level.GetFloorIndices();
    const auto& floorVertices = level.GetFloorVertices();
    
    // Should have 6 indices (2 triangles)
    EXPECT_EQ(6, floorIndices.size());
    
    // All indices should be valid (less than vertex count / 6)
    int vertexCount = floorVertices.size() / 6;
    for (unsigned int index : floorIndices) {
        EXPECT_LT(index, static_cast<unsigned int>(vertexCount));
    }
}

// Test shutdown clears data
TEST_F(LevelTest, ShutdownClearsData) {
    const auto& wallsBefore = level.GetWalls();
    EXPECT_GT(wallsBefore.size(), 0);
    
    level.Shutdown();
    
    const auto& wallsAfter = level.GetWalls();
    EXPECT_EQ(0, wallsAfter.size());
    
    const auto& floorVerticesAfter = level.GetFloorVertices();
    EXPECT_EQ(0, floorVerticesAfter.size());
    
    const auto& floorIndicesAfter = level.GetFloorIndices();
    EXPECT_EQ(0, floorIndicesAfter.size());
}

