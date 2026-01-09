#ifndef FLOOR_RENDERER_H
#define FLOOR_RENDERER_H

#include "../shader/Shader.h"
#include "../level/Level.h"
#include "../Camera.h"
#include <glad/glad.h>

class FloorRenderer {
public:
    FloorRenderer();
    ~FloorRenderer();
    
    bool Initialize();
    void Shutdown();
    void RenderFloor(const Level& level, const Camera& camera, float aspectRatio);
    
private:
    void SetupRendering();
    
    Shader m_shader;
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    bool m_initialized;
};

#endif

