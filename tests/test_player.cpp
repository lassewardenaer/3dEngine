#include <gtest/gtest.h>
#include "gameNodes/characters/Player.h"
#include <glm/gtc/epsilon.hpp>
#include <cmath>

class PlayerTest : public ::testing::Test {
protected:
    void SetUp() override {
        player = Player();
    }
    
    Player player;
    const float EPSILON = 0.001f;
    const float DELTA_TIME = 0.016f; // ~60 FPS
};

// Test default constructor
TEST_F(PlayerTest, DefaultConstructor) {
    glm::vec3 pos = player.GetPosition();
    EXPECT_FLOAT_EQ(0.0f, pos.x);
    EXPECT_FLOAT_EQ(0.5f, pos.y);
    EXPECT_FLOAT_EQ(0.0f, pos.z);
    
    EXPECT_FLOAT_EQ(-90.0f, player.GetYaw());
    EXPECT_FLOAT_EQ(0.0f, player.GetPitch());
    EXPECT_FALSE(player.IsShooting());
}

// Test position setting
TEST_F(PlayerTest, SetPosition) {
    glm::vec3 newPos(5.0f, 1.0f, 3.0f);
    player.SetPosition(newPos);
    
    glm::vec3 pos = player.GetPosition();
    EXPECT_FLOAT_EQ(5.0f, pos.x);
    EXPECT_FLOAT_EQ(1.0f, pos.y);
    EXPECT_FLOAT_EQ(3.0f, pos.z);
}

// Test forward movement
TEST_F(PlayerTest, MoveForward) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    // Default yaw is -90.0f (looking along negative z-axis)
    
    glm::vec3 startPos = player.GetPosition();
    player.MoveForward(DELTA_TIME);
    glm::vec3 endPos = player.GetPosition();
    
    // Should move forward (negative z direction at yaw -90)
    float distance = glm::length(endPos - startPos);
    EXPECT_GT(distance, 0.0f);
    EXPECT_LT(endPos.z, startPos.z); // Moving in negative z
}

// Test backward movement
TEST_F(PlayerTest, MoveBackward) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    // Default yaw is -90.0f
    
    glm::vec3 startPos = player.GetPosition();
    player.MoveBackward(DELTA_TIME);
    glm::vec3 endPos = player.GetPosition();
    
    float distance = glm::length(endPos - startPos);
    EXPECT_GT(distance, 0.0f);
    EXPECT_GT(endPos.z, startPos.z); // Moving in positive z (backward)
}

// Test left movement
TEST_F(PlayerTest, MoveLeft) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    // Default yaw is -90.0f
    
    glm::vec3 startPos = player.GetPosition();
    player.MoveLeft(DELTA_TIME);
    glm::vec3 endPos = player.GetPosition();
    
    float distance = glm::length(endPos - startPos);
    EXPECT_GT(distance, 0.0f);
    EXPECT_LT(endPos.x, startPos.x); // Moving left (negative x)
}

// Test right movement
TEST_F(PlayerTest, MoveRight) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    // Default yaw is -90.0f
    
    glm::vec3 startPos = player.GetPosition();
    player.MoveRight(DELTA_TIME);
    glm::vec3 endPos = player.GetPosition();
    
    float distance = glm::length(endPos - startPos);
    EXPECT_GT(distance, 0.0f);
    EXPECT_GT(endPos.x, startPos.x); // Moving right (positive x)
}

// Test Update stores previous position
TEST_F(PlayerTest, UpdateStoresPreviousPosition) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    glm::vec3 initialPos = player.GetPosition();
    
    player.Update(DELTA_TIME);
    glm::vec3 prevPos = player.GetPreviousPosition();
    
    EXPECT_FLOAT_EQ(initialPos.x, prevPos.x);
    EXPECT_FLOAT_EQ(initialPos.y, prevPos.y);
    EXPECT_FLOAT_EQ(initialPos.z, prevPos.z);
}

// Test mouse movement processing
TEST_F(PlayerTest, ProcessMouseMovement) {
    float initialYaw = player.GetYaw();
    float initialPitch = player.GetPitch();
    
    player.ProcessMouseMovement(10.0f, 5.0f);
    
    EXPECT_NE(initialYaw, player.GetYaw());
    EXPECT_NE(initialPitch, player.GetPitch());
}

// Test pitch clamping in mouse movement
TEST_F(PlayerTest, MouseMovementPitchClamping) {
    player.ProcessMouseMovement(0.0f, 200.0f); // Large upward movement
    EXPECT_LE(player.GetPitch(), 89.0f);
    
    // Reset pitch by processing opposite movement
    player.ProcessMouseMovement(0.0f, -200.0f); // Large downward movement
    EXPECT_GE(player.GetPitch(), -89.0f);
}

// Test shooting
TEST_F(PlayerTest, Shoot) {
    EXPECT_FALSE(player.IsShooting());
    
    player.Shoot(DELTA_TIME);
    EXPECT_TRUE(player.IsShooting());
}

// Test shoot cooldown
TEST_F(PlayerTest, ShootCooldown) {
    player.Shoot(DELTA_TIME);
    EXPECT_TRUE(player.IsShooting());
    
    // Try to shoot again immediately
    player.Shoot(DELTA_TIME);
    // Should still be shooting (cooldown active)
    EXPECT_TRUE(player.IsShooting());
    
    // Wait for cooldown
    float cooldownTime = 0.3f;
    int iterations = static_cast<int>(cooldownTime / DELTA_TIME) + 1;
    for (int i = 0; i < iterations; ++i) {
        player.Update(DELTA_TIME);
    }
    
    EXPECT_FALSE(player.IsShooting());
}

// Test movement speed
TEST_F(PlayerTest, MovementSpeed) {
    player.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    // Rotate to look along positive x (yaw 0.0f from default -90.0f)
    player.ProcessMouseMovement(90.0f, 0.0f);
    
    glm::vec3 startPos = player.GetPosition();
    player.MoveForward(1.0f); // Move for 1 second
    glm::vec3 endPos = player.GetPosition();
    
    float distance = glm::length(endPos - startPos);
    // Speed should be 5.0f, so distance should be approximately 5.0
    EXPECT_NEAR(5.0f, distance, EPSILON);
}

