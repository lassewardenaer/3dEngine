#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include <glad/glad.h>

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    void Shutdown();
    
    void RenderLevel(const Level& level, const Camera& camera, float aspectRatio);
    void RenderPlayer(const Player& player, const Camera& camera);
    
private:
    void SetupWallRendering();
    void SetupFloorRendering();
    void SetupSkyRendering();
    void RenderWall(const Wall& wall, const Camera& camera);
    void RenderSky(const Camera& camera, float aspectRatio);
    
    Shader m_wallShader;
    Shader m_floorShader;
    Shader m_skyShader;
    
    unsigned int m_wallVAO;
    unsigned int m_wallVBO;
    unsigned int m_wallEBO;
    unsigned int m_floorVAO;
    unsigned int m_floorVBO;
    unsigned int m_floorEBO;
    unsigned int m_skyVAO;
    unsigned int m_skyVBO;
    unsigned int m_skyEBO;
    
    bool m_initialized;
};

#endif

