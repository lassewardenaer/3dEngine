//create an enemy class
#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {
public:
    Enemy();
    Enemy(const glm::vec3& position);
    ~Enemy();
};

#endif