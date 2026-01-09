//create a bullet class
#ifndef BULLET_H
#define BULLET_H

#include <glm/glm.hpp>

enum class BulletType {
    PLAYER,
    ENEMY
};

class Bullet {
public:
    Bullet(const glm::vec3& position, const glm::vec3& velocity, float lifetime);
    ~Bullet();

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float m_lifetime;
    glm::vec3 m_color;
    float m_size;
    BulletType m_type;
};

#endif