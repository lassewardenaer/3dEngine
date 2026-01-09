#include "WallRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

WallRenderer::WallRenderer()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_initialized(false)
{
}

WallRenderer::~WallRenderer() {
    Shutdown();
}

bool WallRenderer::Initialize() {
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
        
        uniform vec3 lightPos;
        uniform vec3 lightColor;
        uniform vec3 viewPos;
        uniform float shadowIntensity;
        
        float calculateShadow(vec3 fragPos, vec3 normal, vec3 lightDir) {
            float distance = length(lightPos - fragPos);
            float shadowFactor = 1.0;
            
            float NdotL = dot(normalize(normal), normalize(lightDir));
            if (NdotL < 0.0) {
                shadowFactor = 0.2;
            } else {
                shadowFactor = mix(0.3, 1.0, NdotL);
            }
            
            float maxDistance = 20.0;
            float distanceFactor = 1.0 - smoothstep(0.0, maxDistance, distance);
            shadowFactor *= mix(0.4, 1.0, distanceFactor);
            
            return mix(1.0, shadowFactor, shadowIntensity);
        }
        
        void main() {
            vec3 color = vec3(0.8, 0.2, 0.2);
            
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            
            float shadow = calculateShadow(FragPos, norm, lightDir);
            
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor * shadow;
            
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor * shadow;
            
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

void WallRenderer::Shutdown() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
    }
}

void WallRenderer::SetupRendering() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void WallRenderer::RenderWalls(const Level& level, const Camera& camera, float aspectRatio) {
    m_shader.Use();
    m_shader.SetMat4("view", camera.GetViewMatrix());
    m_shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_shader.SetVec3("lightPos", glm::vec3(0.0f, 8.0f, 5.0f));
    m_shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.SetVec3("viewPos", camera.GetPosition());
    m_shader.SetFloat("shadowIntensity", 0.8f);
    
    glBindVertexArray(m_VAO);
    
    for (const auto& wall : level.GetWalls()) {
        RenderWall(wall, camera);
    }
    
    glBindVertexArray(0);
}

void WallRenderer::RenderWall(const Wall& wall, const Camera& camera) {
    glm::vec3 wallDir = wall.end - wall.start;
    glm::vec3 wallNormal = glm::normalize(glm::vec3(-wallDir.z, 0.0f, wallDir.x));
    
    float vertices[] = {
        wall.start.x, 0.0f, wall.start.z,  wallNormal.x, wallNormal.y, wallNormal.z,
        wall.end.x, 0.0f, wall.end.z,      wallNormal.x, wallNormal.y, wallNormal.z,
        wall.end.x, wall.height, wall.end.z, wallNormal.x, wallNormal.y, wallNormal.z,
        wall.start.x, wall.height, wall.start.z, wallNormal.x, wallNormal.y, wallNormal.z,
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glm::mat4 model = glm::mat4(1.0f);
    m_shader.SetMat4("model", model);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

