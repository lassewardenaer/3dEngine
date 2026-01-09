#include "BulletRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>

BulletRenderer::BulletRenderer()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_indexCount(0)
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
        layout (location = 1) in vec3 aNormal;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 FragPos;
        out vec3 Normal;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";
    
    const char* fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        
        uniform vec3 color;
        uniform vec3 lightPos;
        uniform vec3 viewPos;
        
        void main() {
            // Simple Phong lighting for 3D effect
            vec3 lightColor = vec3(1.0, 1.0, 1.0);
            
            // Ambient
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * lightColor;
            
            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            // Specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;
            
            vec3 result = (ambient + diffuse + specular) * color;
            FragColor = vec4(result, 1.0);
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
    // Generate sphere geometry for bullets
    const int segments = 16;  // Number of segments for sphere (higher = smoother)
    const int rings = 16;      // Number of rings for sphere
    
    std::vector<float> bulletVertices;
    std::vector<unsigned int> bulletIndices;
    
    // Generate sphere vertices (position + normal)
    for (int i = 0; i <= rings; ++i) {
        float theta = i * 3.14159265f / rings;  // Vertical angle
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int j = 0; j <= segments; ++j) {
            float phi = j * 2.0f * 3.14159265f / segments;  // Horizontal angle
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            // Position (normalized, will be scaled by bullet size)
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            
            // Add vertex (position + normal, both are the same for a unit sphere)
            bulletVertices.push_back(x);
            bulletVertices.push_back(y);
            bulletVertices.push_back(z);
            bulletVertices.push_back(x);  // normal x
            bulletVertices.push_back(y);  // normal y
            bulletVertices.push_back(z);  // normal z
        }
    }
    
    // Generate sphere indices
    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;
            
            // First triangle
            bulletIndices.push_back(first);
            bulletIndices.push_back(second);
            bulletIndices.push_back(first + 1);
            
            // Second triangle
            bulletIndices.push_back(second);
            bulletIndices.push_back(second + 1);
            bulletIndices.push_back(first + 1);
        }
    }
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, bulletVertices.size() * sizeof(float), bulletVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bulletIndices.size() * sizeof(unsigned int), bulletIndices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    // Store index count for rendering
    m_indexCount = bulletIndices.size();
}

void BulletRenderer::RenderBullet(const Bullet& bullet, const Camera& camera, float aspectRatio) {
    m_shader.Use();
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, bullet.m_position);
    model = glm::scale(model, glm::vec3(bullet.m_size));
    
    glm::vec3 cameraPos = camera.GetPosition();
    glm::vec3 lightPos = cameraPos + glm::vec3(5.0f, 5.0f, 5.0f);  // Light near camera
    
    m_shader.SetMat4("model", model);
    m_shader.SetMat4("view", camera.GetViewMatrix());
    m_shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_shader.SetVec3("color", bullet.m_color);
    m_shader.SetVec3("lightPos", lightPos);
    m_shader.SetVec3("viewPos", cameraPos);
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

