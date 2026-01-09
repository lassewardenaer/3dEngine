#include "SkyRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SkyRenderer::SkyRenderer()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_initialized(false)
{
}

SkyRenderer::~SkyRenderer() {
    Shutdown();
}

bool SkyRenderer::Initialize() {
    const char* vertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 WorldPos;
        
        void main() {
            WorldPos = aPos;
            vec4 pos = projection * view * vec4(aPos, 1.0);
            gl_Position = pos.xyww;
        }
    )";
    
    const char* fragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 WorldPos;
        
        void main() {
            vec3 topColor = vec3(0.2, 0.4, 0.8);
            vec3 bottomColor = vec3(0.6, 0.7, 0.9);
            
            vec3 dir = normalize(WorldPos);
            float t = (dir.y + 1.0) * 0.5;
            t = smoothstep(0.0, 1.0, t);
            
            vec3 color = mix(bottomColor, topColor, t);
            
            float horizon = abs(dir.y);
            if (horizon < 0.2) {
                float hazeAmount = (0.2 - horizon) / 0.2;
                color = mix(color, vec3(0.8, 0.8, 0.9), hazeAmount * 0.3);
            }
            
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

void SkyRenderer::Shutdown() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_VAO = 0;
        m_VBO = 0;
        m_EBO = 0;
    }
}

void SkyRenderer::SetupRendering() {
    float skySize = 100.0f;
    float skyVertices[] = {
        -skySize, -skySize, -skySize,
         skySize, -skySize, -skySize,
         skySize,  skySize, -skySize,
        -skySize,  skySize, -skySize,
        
        -skySize, -skySize,  skySize,
         skySize, -skySize,  skySize,
         skySize,  skySize,  skySize,
        -skySize,  skySize,  skySize,
    };
    
    unsigned int skyIndices[] = {
        0, 3, 2, 2, 1, 0,
        4, 5, 6, 6, 7, 4,
        3, 7, 6, 6, 2, 3,
        1, 2, 6, 6, 5, 1,
        0, 4, 7, 7, 3, 0,
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyIndices), skyIndices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void SkyRenderer::RenderSky(const Camera& camera, float aspectRatio) {
    GLboolean depthMaskEnabled;
    GLint depthFuncInt;
    GLboolean cullFaceEnabled;
    GLboolean depthTestEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFuncInt);
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    
    m_shader.Use();
    
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    m_shader.SetMat4("view", view);
    m_shader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    glDepthMask(depthMaskEnabled);
    glDepthFunc(static_cast<GLenum>(depthFuncInt));
    if (cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    }
    glBindVertexArray(0);
}

