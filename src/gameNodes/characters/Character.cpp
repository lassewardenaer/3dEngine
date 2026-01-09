#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

#include "Character.h"
#include "weapons/Bullet.h"

Character::Character()
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

void Character::Update(float deltaTime) {
    m_previousPosition = m_position;
    
    if (m_shootTimer > 0.0f) {
        m_shootTimer -= deltaTime;
        if (m_shootTimer <= 0.0f) {
            m_shooting = false;
        }
    }
    
    // Update bullets: move them and remove expired ones
    for (auto it = m_bullets.begin(); it != m_bullets.end();) {
        // Move bullet
        it->m_position += it->m_velocity * deltaTime;
        
        // Decrease lifetime
        it->m_lifetime -= deltaTime;
        
        // Remove if expired
        if (it->m_lifetime <= 0.0f) {
            it = m_bullets.erase(it);
        } else {
            ++it;
        }
    }
}

void Character::MoveForward(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
    m_position += direction * m_speed * deltaTime;
}

void Character::MoveBackward(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
    m_position -= direction * m_speed * deltaTime;
}

void Character::MoveLeft(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
    m_position -= direction * m_speed * deltaTime;
}

void Character::MoveRight(float deltaTime) {
    float yawRad = glm::radians(m_yaw);
    glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
    m_position += direction * m_speed * deltaTime;
}

void Character::Shoot(const float& deltaTime) {
    if (m_shootTimer <= 0.0f) {
        m_shooting = true;
        m_shootTimer = m_shootCooldown;
        
        // Calculate bullet direction based on yaw and pitch (forward direction)
        float yawRad = glm::radians(m_yaw);
        float pitchRad = glm::radians(m_pitch);
        glm::vec3 bulletDirection(
            cos(yawRad) * cos(pitchRad),
            sin(pitchRad),
            sin(yawRad) * cos(pitchRad)
        );
        bulletDirection = glm::normalize(bulletDirection);
        
        // Create bullet at character's position
        glm::vec3 bulletPosition = m_position;
        glm::vec3 bulletVelocity = bulletDirection * 20.0f; // Bullet speed
        float bulletLifetime = 3.0f; // Bullets last 3 seconds
        
        m_bullets.push_back(Bullet(bulletPosition, bulletVelocity, bulletLifetime));
        m_bullets.back().m_lifetime = bulletLifetime;
        m_bullets.back().m_color = glm::vec3(1.0f, 0.0f, 0.0f); // Red bullets
               m_bullets.back().m_size = 0.1f; // Bullet size (smaller ball)
    } else {
        m_shootTimer -= deltaTime;
    }
}

