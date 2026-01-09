//create a bullet class
#include "Bullet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Bullet::Bullet(const glm::vec3& position, const glm::vec3& velocity, float lifetime)
    : m_position(position)
    , m_velocity(velocity)
    , m_lifetime(lifetime)
    , m_color(1.0f, 1.0f, 1.0f)  // Default white
    , m_size(0.1f)                // Default size
    , m_type(BulletType::PLAYER)  // Default type
{
}

Bullet::~Bullet() {
    // Destructor implementation (empty for now)
}