#ifndef CHARACTER_H
#define CHARACTER_H

#include <glm/glm.hpp>
#include <vector>
#include "weapons/Bullet.h"

class Character {
public:
    Character();
    
    void Update(float deltaTime);
    void MoveForward(float deltaTime);
    void MoveBackward(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void Shoot(const float& deltaTime);
    
    glm::vec3 GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& position) { m_position = position; }
    glm::vec3 GetPreviousPosition() const { return m_previousPosition; }
    
    float GetYaw() const { return m_yaw; }
    float GetPitch() const { return m_pitch; }
    float GetSpeed() const { return m_speed; }
    
    bool IsShooting() const { return m_shooting; }
    void SetShooting(bool shooting) { m_shooting = shooting; }
    
    const std::vector<Bullet>& GetBullets() const { return m_bullets; }
    
protected:
    glm::vec3 m_position;
    glm::vec3 m_previousPosition;
    float m_yaw;
    float m_pitch;
    
    float m_speed;
    float m_mouseSensitivity;
    
    bool m_shooting;
    float m_shootCooldown;
    float m_shootTimer;
    std::vector<Bullet> m_bullets;
};

#endif

