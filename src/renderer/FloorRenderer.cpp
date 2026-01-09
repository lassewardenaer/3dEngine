#include "FloorRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

FloorRenderer::FloorRenderer()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_initialized(false)
{
}

FloorRenderer::~FloorRenderer() {
    Shutdown();
}

bool FloorRenderer::Initialize() {
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
            vec4 worldPos = model * vec4(aPos, 1.0);
            FragPos = vec3(worldPos);
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * worldPos;
        }
    )";
    
    const char* fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        
        uniform vec3 lightPos;
        uniform vec3 lightColor;
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
            vec2 grid = floor(FragPos.xz * 0.5);
            float checker = mod(grid.x + grid.y, 2.0);
            vec3 baseColor = mix(vec3(0.6, 0.4, 0.2), vec3(0.5, 0.35, 0.15), checker);
            
            float ambientStrength = 0.6;
            vec3 ambient = ambientStrength * lightColor;
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            
            float shadow = calculateShadow(FragPos, norm, lightDir);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor * shadow;
            
            vec3 lighting = ambient + diffuse;
            vec3 result = baseColor * lighting;
            result = max(result, baseColor * 0.3);
            
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

void FloorRenderer::Shutdown() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
    }
}

void FloorRenderer::SetupRendering() {
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

void FloorRenderer::RenderFloor(const Level& level, const Camera& camera, float aspectRatio) {
    m_shader.Use();
    
    const auto& floorVertices = level.GetFloorVertices();
    const auto& floorIndices = level.GetFloorIndices();
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, floorVertices.size() * sizeof(float), floorVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floorIndices.size() * sizeof(unsigned int), floorIndices.data(), GL_STATIC_DRAW);
    
    GLboolean cullFaceEnabled;
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
    glDisable(GL_CULL_FACE);
    
    glm::mat4 model = glm::mat4(1.0f);
    m_shader.SetMat4("model", model);
    m_shader.SetMat4("view", camera.GetViewMatrix());
    m_shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_shader.SetVec3("lightPos", glm::vec3(0.0f, 8.0f, 5.0f));
    m_shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_shader.SetFloat("shadowIntensity", 0.8f);
    
    glDrawElements(GL_TRIANGLES, floorIndices.size(), GL_UNSIGNED_INT, 0);
    
    if (cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    }
}

