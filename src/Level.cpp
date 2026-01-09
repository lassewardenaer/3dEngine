#include "Level.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Level::Level() : m_levelSize(20.0f) {
}

Level::~Level() {
    Shutdown();
}

bool Level::Initialize() {
    CreateWalls();
    CreateFloor();
    return true;
}

void Level::Shutdown() {
    m_walls.clear();
    m_floorVertices.clear();
    m_floorIndices.clear();
}

void Level::CreateWalls() {
    // Create a simple maze-like level
    float wallHeight = 2.0f;
    
    // Outer walls
    m_walls.push_back({{-m_levelSize, 0, -m_levelSize}, {m_levelSize, 0, -m_levelSize}, wallHeight});
    m_walls.push_back({{m_levelSize, 0, -m_levelSize}, {m_levelSize, 0, m_levelSize}, wallHeight});
    m_walls.push_back({{m_levelSize, 0, m_levelSize}, {-m_levelSize, 0, m_levelSize}, wallHeight});
    m_walls.push_back({{-m_levelSize, 0, m_levelSize}, {-m_levelSize, 0, -m_levelSize}, wallHeight});
    
    // Inner walls
    m_walls.push_back({{-5.0f, 0, -5.0f}, {-5.0f, 0, 5.0f}, wallHeight});
    m_walls.push_back({{5.0f, 0, -5.0f}, {5.0f, 0, 5.0f}, wallHeight});
    m_walls.push_back({{-10.0f, 0, 0.0f}, {10.0f, 0, 0.0f}, wallHeight});
}

void Level::CreateFloor() {
    // Floor vertices (position + normal)
    float size = m_levelSize;
    m_floorVertices = {
        // Position          // Normal
        -size, 0.0f, -size,  0.0f, 1.0f, 0.0f,
         size, 0.0f, -size,  0.0f, 1.0f, 0.0f,
         size, 0.0f,  size,  0.0f, 1.0f, 0.0f,
        -size, 0.0f,  size,  0.0f, 1.0f, 0.0f,
    };
    
    m_floorIndices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };
}

bool Level::CheckCollision(const glm::vec3& position, float radius) const {
    for (const auto& wall : m_walls) {
        glm::vec3 wallDir = wall.end - wall.start;
        float wallLength = glm::length(wallDir);
        glm::vec3 wallNormal = glm::normalize(wallDir);
        
        glm::vec3 toPlayer = position - wall.start;
        float projection = glm::dot(toPlayer, wallNormal);
        
        if (projection < 0) projection = 0;
        if (projection > wallLength) projection = wallLength;
        
        glm::vec3 closestPoint = wall.start + wallNormal * projection;
        float distance = glm::length(position - closestPoint);
        
        if (distance < radius) {
            return true;
        }
    }
    
    // Check boundaries
    if (position.x < -m_levelSize + radius || position.x > m_levelSize - radius ||
        position.z < -m_levelSize + radius || position.z > m_levelSize - radius) {
        return true;
    }
    
    return false;
}

