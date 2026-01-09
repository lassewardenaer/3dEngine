//create an enemy class
#ifndef ENEMY_H
#define ENEMY_H

#include <glm/glm.hpp>
#include "Character.h"

class Enemy : public Character {
public:
    Enemy();
    Enemy(const glm::vec3& position);
    ~Enemy();
};

#endif

