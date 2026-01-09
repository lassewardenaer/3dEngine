#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include "Character.h"

class Player : public Character {
public:
    Player();
    Player(const glm::vec3& position);
    
    void ProcessMouseMovement(float xoffset, float yoffset);
};

#endif

