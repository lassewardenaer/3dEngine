#include "Renderer.h"
#include "../gameNodes/characters/Enemy.h"

Renderer::Renderer()
    : m_initialized(false)
{
}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize() {
    if (!m_wallRenderer.Initialize()) {
        return false;
    }
    if (!m_floorRenderer.Initialize()) {
        return false;
    }
    if (!m_skyRenderer.Initialize()) {
        return false;
    }
    if (!m_bulletRenderer.Initialize()) {
        return false;
    }
    
    m_initialized = true;
    return true;
}

void Renderer::Shutdown() {
    m_wallRenderer.Shutdown();
    m_floorRenderer.Shutdown();
    m_skyRenderer.Shutdown();
    m_bulletRenderer.Shutdown();
    m_initialized = false;
}

void Renderer::RenderLevel(const Level& level, const Camera& camera, float aspectRatio) {
    // Render sky first (background)
    m_skyRenderer.RenderSky(camera, aspectRatio);
    
    // Re-enable depth testing for floor and walls
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    
    // Render floor
    m_floorRenderer.RenderFloor(level, camera, aspectRatio);
    
    // Render walls
    m_wallRenderer.RenderWalls(level, camera, aspectRatio);
}

void Renderer::RenderPlayer(const Player& player, const Camera& camera, float aspectRatio) {
    // Player rendering can be added here (weapon, crosshair, etc.)
    // Render player bullets
    RenderPlayerBullets(player, camera, aspectRatio);
}

void Renderer::RenderPlayerBullets(const Player& player, const Camera& camera, float aspectRatio) {
    for (const auto& bullet : player.GetBullets()) {
        m_bulletRenderer.RenderBullet(bullet, camera, aspectRatio);
    }
}

void Renderer::RenderEnemyBullets(const Enemy& enemy, const Camera& camera, float aspectRatio) {
    for (const auto& bullet : enemy.GetBullets()) {
        m_bulletRenderer.RenderBullet(bullet, camera, aspectRatio);
    }
}

