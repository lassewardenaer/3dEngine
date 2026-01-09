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
    // Project position to XZ plane (y=0) for horizontal collision detection
    glm::vec2 pos2D(position.x, position.z);
    
    for (const auto& wall : m_walls) {
        // Project wall to XZ plane
        glm::vec2 wallStart(wall.start.x, wall.start.z);
        glm::vec2 wallEnd(wall.end.x, wall.end.z);
        
        glm::vec2 wallDir = wallEnd - wallStart;
        float wallLength = glm::length(wallDir);
        
        // Skip zero-length walls
        if (wallLength < 0.001f) continue;
        
        glm::vec2 wallNormal = glm::normalize(wallDir);
        
        glm::vec2 toPlayer = pos2D - wallStart;
        float projection = glm::dot(toPlayer, wallNormal);
        
        // Clamp projection to wall segment
        if (projection < 0) projection = 0;
        if (projection > wallLength) projection = wallLength;
        
        // Find closest point on wall segment
        glm::vec2 closestPoint = wallStart + wallNormal * projection;
        
        // Calculate 2D distance (ignoring Y coordinate)
        float distance = glm::length(pos2D - closestPoint);
        
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

