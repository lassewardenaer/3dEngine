#ifndef SKY_RENDERER_H
#define SKY_RENDERER_H

#include "../shader/Shader.h"
#include "../Camera.h"
#include <glad/glad.h>

class SkyRenderer {
public:
    SkyRenderer();
    ~SkyRenderer();
    
    bool Initialize();
    void Shutdown();
    void RenderSky(const Camera& camera, float aspectRatio);
    
private:
    void SetupRendering();
    
    Shader m_shader;
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    bool m_initialized;
};

#endif

