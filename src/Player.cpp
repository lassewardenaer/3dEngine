#include "Player.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Player::Player()
    : m_position(0.0f, 0.5f, 2.0f)
    , m_previousPosition(0.0f, 0.5f, 2.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_speed(5.0f)
    , m_mouseSensitivity(0.1f)
    , m_shooting(false)
    , m_shootCooldown(0.3f)
    , m_shootTimer(0.0f)
{
}

void Player::Update(float deltaTime) {
    m_previousPosition = m_position;
    
    if (m_shootTimer > 0.0f) {
        m_shootTimer -= deltaTime;
        if (m_shootTimer <= 0.0f) {
            m_shooting = false;
        }
    }
}

void Player::MoveForward(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
    m_position += direction * m_speed * deltaTime;
}

void Player::MoveBackward(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
    m_position -= direction * m_speed * deltaTime;
}

void Player::MoveLeft(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
    m_position -= direction * m_speed * deltaTime;
}

void Player::MoveRight(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
    m_position += direction * m_speed * deltaTime;
}

void Player::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;
    
    m_yaw += xoffset;
    m_pitch += yoffset;
    
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
}

void Player::Shoot(const float& deltaTime) {
    if (m_shootTimer <= 0.0f) {
        m_shooting = true;
        m_shootTimer = m_shootCooldown;
    } else {
        m_shootTimer -= deltaTime;
    }
}

