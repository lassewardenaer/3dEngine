#ifndef RENDERER_H
#define RENDERER_H

#include "WallRenderer.h"
#include "FloorRenderer.h"
#include "SkyRenderer.h"
#include "BulletRenderer.h"
#include "../gameNodes/characters/Player.h"
#include <glad/glad.h>

// Forward declaration
class Enemy;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize();
    void Shutdown();
    
    void RenderLevel(const Level& level, const Camera& camera, float aspectRatio);
    void RenderPlayer(const Player& player, const Camera& camera, float aspectRatio);
    void RenderPlayerBullets(const Player& player, const Camera& camera, float aspectRatio);
    void RenderEnemyBullets(const Enemy& enemy, const Camera& camera, float aspectRatio);
    
private:
    WallRenderer m_wallRenderer;
    FloorRenderer m_floorRenderer;
    SkyRenderer m_skyRenderer;
    BulletRenderer m_bulletRenderer;
    
    bool m_initialized;
};

#endif

