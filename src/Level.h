#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <glm/glm.hpp>

struct Wall {
    glm::vec3 start;
    glm::vec3 end;
    float height;
};

class Level {
public:
    Level();
    ~Level();
    
    bool Initialize();
    void Shutdown();
    
    const std::vector<Wall>& GetWalls() const { return m_walls; }
    const std::vector<float>& GetFloorVertices() const { return m_floorVertices; }
    const std::vector<unsigned int>& GetFloorIndices() const { return m_floorIndices; }
    
    bool CheckCollision(const glm::vec3& position, float radius) const;
    
private:
    void CreateWalls();
    void CreateFloor();
    
    std::vector<Wall> m_walls;
    std::vector<float> m_floorVertices;
    std::vector<unsigned int> m_floorIndices;
    
    float m_levelSize;
};

#endif

