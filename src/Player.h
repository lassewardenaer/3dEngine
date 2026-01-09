#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

class Player {
public:
    Player();
    
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
    
    bool IsShooting() const { return m_shooting; }
    void SetShooting(bool shooting) { m_shooting = shooting; }
    
private:
    glm::vec3 m_position;
    glm::vec3 m_previousPosition;
    float m_yaw;
    float m_pitch;
    
    float m_speed;
    float m_mouseSensitivity;
    
    bool m_shooting;
    float m_shootCooldown;
    float m_shootTimer;
};

#endif

