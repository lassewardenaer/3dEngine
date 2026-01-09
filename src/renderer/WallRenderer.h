#ifndef WALL_RENDERER_H
#define WALL_RENDERER_H

#include "../shader/Shader.h"
#include "../level/Level.h"
#include "../Camera.h"
#include <glad/glad.h>

class WallRenderer {
public:
    WallRenderer();
    ~WallRenderer();
    
    bool Initialize();
    void Shutdown();
    void RenderWalls(const Level& level, const Camera& camera, float aspectRatio);
    
private:
    void SetupRendering();
    void RenderWall(const Wall& wall, const Camera& camera);
    
    Shader m_shader;
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    bool m_initialized;
};

#endif

