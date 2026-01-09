#include "BulletRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BulletRenderer::BulletRenderer()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_initialized(false)
{
}

BulletRenderer::~BulletRenderer() {
    Shutdown();
}

bool BulletRenderer::Initialize() {
    const char* vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    const char* fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        uniform vec3 color;
        
        void main() {
            FragColor = vec4(color, 1.0);
        }
    )";
    
    if (!m_shader.LoadFromSource(vertexShader, fragmentShader)) {
        return false;
    }
    
    SetupRendering();
    m_initialized = true;
    return true;
}

void BulletRenderer::Shutdown() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
    }
}

void BulletRenderer::SetupRendering() {
    float bulletSize = 0.1f;
    float bulletVertices[] = {
        -bulletSize, -bulletSize,  bulletSize,
         bulletSize, -bulletSize,  bulletSize,
         bulletSize,  bulletSize,  bulletSize,
        -bulletSize,  bulletSize,  bulletSize,
        -bulletSize, -bulletSize, -bulletSize,
         bulletSize, -bulletSize, -bulletSize,
         bulletSize,  bulletSize, -bulletSize,
        -bulletSize,  bulletSize, -bulletSize,
    };
    
    unsigned int bulletIndices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        3, 2, 6, 6, 7, 3,
        0, 1, 5, 5, 4, 0,
        1, 5, 6, 6, 2, 1,
        0, 4, 7, 7, 3, 0,
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bulletIndices), bulletIndices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void BulletRenderer::RenderBullet(const Bullet& bullet, const Camera& camera, float aspectRatio) {
    m_shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, bullet.m_position);
    model = glm::scale(model, glm::vec3(bullet.m_size));
    
    m_shader.SetMat4("model", model);
    m_shader.SetMat4("view", camera.GetViewMatrix());
    m_shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_shader.SetVec3("color", bullet.m_color);
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

