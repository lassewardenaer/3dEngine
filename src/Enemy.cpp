//create an enemy class
#include "Enemy.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Enemy::Enemy() : Character() {
    // Enemy-specific initialization
}

Enemy::Enemy(const glm::vec3& position) : Character() {
    SetPosition(position);
}

Enemy::~Enemy() {
    // Destructor
}