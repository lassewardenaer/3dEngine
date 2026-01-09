#include <gtest/gtest.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

class CameraTest : public ::testing::Test {
protected:
    void SetUp() override {
        camera = Camera();
    }
    
    Camera camera;
    const float EPSILON = 0.001f;
};

// Test default constructor
TEST_F(CameraTest, DefaultConstructor) {
    glm::vec3 pos = camera.GetPosition();
    EXPECT_FLOAT_EQ(0.0f, pos.x);
    EXPECT_FLOAT_EQ(0.5f, pos.y);
    EXPECT_FLOAT_EQ(0.0f, pos.z);
}

// Test position setting
TEST_F(CameraTest, SetPosition) {
    glm::vec3 newPos(1.0f, 2.0f, 3.0f);
    camera.SetPosition(newPos);
    
    glm::vec3 pos = camera.GetPosition();
    EXPECT_FLOAT_EQ(1.0f, pos.x);
    EXPECT_FLOAT_EQ(2.0f, pos.y);
    EXPECT_FLOAT_EQ(3.0f, pos.z);
}

// Test yaw setting
TEST_F(CameraTest, SetYaw) {
    camera.SetYaw(45.0f);
    glm::vec3 front = camera.GetFront();
    
    // At yaw 45 degrees, front should point in positive x and z direction
    EXPECT_GT(front.x, 0.0f);
    EXPECT_GT(front.z, 0.0f);
}

// Test pitch clamping
TEST_F(CameraTest, PitchClamping) {
    camera.SetPitch(100.0f);
    // Pitch should be clamped to 89.0f
    // We can't directly access pitch, but we can check the front vector
    glm::vec3 front = camera.GetFront();
    EXPECT_LE(front.y, sin(glm::radians(89.0f)) + EPSILON);
    
    camera.SetPitch(-100.0f);
    front = camera.GetFront();
    EXPECT_GE(front.y, sin(glm::radians(-89.0f)) - EPSILON);
}

// Test view matrix
TEST_F(CameraTest, ViewMatrix) {
    camera.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
    camera.SetYaw(-90.0f);
    camera.SetPitch(0.0f);
    
    glm::mat4 view = camera.GetViewMatrix();
    
    // View matrix should be invertible (determinant != 0)
    float det = glm::determinant(view);
    EXPECT_GT(std::abs(det), 0.001f);
}

// Test projection matrix
TEST_F(CameraTest, ProjectionMatrix) {
    float aspectRatio = 16.0f / 9.0f;
    glm::mat4 proj = camera.GetProjectionMatrix(aspectRatio);
    
    // Projection matrix should be invertible
    float det = glm::determinant(proj);
    EXPECT_GT(std::abs(det), 0.001f);
}

// Test front vector normalization
TEST_F(CameraTest, FrontVectorNormalized) {
    camera.SetYaw(45.0f);
    camera.SetPitch(30.0f);
    
    glm::vec3 front = camera.GetFront();
    float length = glm::length(front);
    EXPECT_NEAR(1.0f, length, EPSILON);
}

// Test right vector
TEST_F(CameraTest, RightVector) {
    camera.SetYaw(0.0f);
    camera.SetPitch(0.0f);
    
    glm::vec3 right = camera.GetRight();
    float length = glm::length(right);
    EXPECT_NEAR(1.0f, length, EPSILON);
}

// Test up vector
TEST_F(CameraTest, UpVector) {
    camera.SetYaw(45.0f);
    camera.SetPitch(30.0f);
    
    glm::vec3 up = camera.GetUp();
    float length = glm::length(up);
    EXPECT_NEAR(1.0f, length, EPSILON);
}

// Test vector orthogonality
TEST_F(CameraTest, VectorOrthogonality) {
    camera.SetYaw(45.0f);
    camera.SetPitch(30.0f);
    
    glm::vec3 front = camera.GetFront();
    glm::vec3 right = camera.GetRight();
    glm::vec3 up = camera.GetUp();
    
    // Front and right should be perpendicular
    float dotFR = glm::dot(front, right);
    EXPECT_NEAR(0.0f, dotFR, EPSILON);
    
    // Right and up should be perpendicular
    float dotRU = glm::dot(right, up);
    EXPECT_NEAR(0.0f, dotRU, EPSILON);
}

