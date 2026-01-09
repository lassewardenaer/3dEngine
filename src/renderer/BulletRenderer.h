#ifndef BULLET_RENDERER_H
#define BULLET_RENDERER_H

#include "../shader/Shader.h"
#include "../gameNodes/weapons/Bullet.h"
#include "../Camera.h"
#include <glad/glad.h>

class BulletRenderer {
public:
    BulletRenderer();
    ~BulletRenderer();
    
    bool Initialize();
    void Shutdown();
    void RenderBullet(const Bullet& bullet, const Camera& camera, float aspectRatio);
    
private:
    void SetupRendering();
    
    Shader m_shader;
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
    unsigned int m_indexCount;
    bool m_initialized;
};

#endif

