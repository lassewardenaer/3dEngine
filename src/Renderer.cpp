#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

Renderer::Renderer()
    : m_wallVAO(0)
    , m_wallVBO(0)
    , m_wallEBO(0)
    , m_floorVAO(0)
    , m_floorVBO(0)
    , m_floorEBO(0)
    , m_skyVAO(0)
    , m_skyVBO(0)
    , m_skyEBO(0)
    , m_initialized(false)
{
}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize() {
    // Wall shader
    const char* wallVertexShader = R"(
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
    
    const char* wallFragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        
        uniform vec3 lightPos;
        uniform vec3 lightColor;
        uniform vec3 viewPos;
        uniform float shadowIntensity;
        
        float calculateShadow(vec3 fragPos, vec3 normal, vec3 lightDir) {
            // Simple shadow calculation based on distance and angle
            float distance = length(lightPos - fragPos);
            float shadowFactor = 1.0;
            
            // Shadow based on angle - surfaces facing away from light are darker
            float NdotL = dot(normalize(normal), normalize(lightDir));
            if (NdotL < 0.0) {
                // Surface is facing away from light - in shadow
                shadowFactor = 0.2;
            } else {
                // Gradual shadow based on angle - more perpendicular = darker
                shadowFactor = mix(0.3, 1.0, NdotL);
            }
            
            // Distance-based shadow falloff
            float maxDistance = 20.0;
            float distanceFactor = 1.0 - smoothstep(0.0, maxDistance, distance);
            shadowFactor *= mix(0.4, 1.0, distanceFactor);
            
            // Apply shadow intensity (shadowIntensity: 0.0 = no shadows, 1.0 = full shadows)
            // Higher shadowIntensity means more shadow effect
            return mix(1.0, shadowFactor, shadowIntensity);
        }
        
        void main() {
            vec3 color = vec3(0.8, 0.2, 0.2);
            
            // Ambient
            float ambientStrength = 0.2;
            vec3 ambient = ambientStrength * lightColor;
            
            // Calculate light direction
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            
            // Calculate shadow
            float shadow = calculateShadow(FragPos, norm, lightDir);
            
            // Diffuse
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor * shadow;
            
            // Specular
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor * shadow;
            
            vec3 result = (ambient + diffuse + specular) * color;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    if (!m_wallShader.LoadFromSource(wallVertexShader, wallFragmentShader)) {
        return false;
    }
    
    // Floor shader
    const char* floorVertexShader = R"(
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
            // For floor, model is identity, so normal doesn't need transformation
            // But we transform it anyway for consistency and in case model changes
            Normal = mat3(transpose(inverse(model))) * aNormal;
            gl_Position = projection * view * worldPos;
        }
    )";
    
    const char* floorFragmentShader = R"(
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
            
            // Apply shadow intensity (shadowIntensity: 0.0 = no shadows, 1.0 = full shadows)
            return mix(1.0, shadowFactor, shadowIntensity);
        }
        
        void main() {
            // Create a checkerboard pattern for the floor
            vec2 grid = floor(FragPos.xz * 0.5);
            float checker = mod(grid.x + grid.y, 2.0);
            // Brown floor color - mix between two brown shades for checkerboard
            vec3 baseColor = mix(vec3(0.6, 0.4, 0.2), vec3(0.5, 0.35, 0.15), checker);
            
            float ambientStrength = 0.6;
            vec3 ambient = ambientStrength * lightColor;
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            
            float shadow = calculateShadow(FragPos, norm, lightDir);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor * shadow;
            
            // Mix the lighting with the base color more directly
            vec3 lighting = ambient + diffuse;
            vec3 result = baseColor * lighting;
            
            // Ensure minimum brightness so base color is visible
            result = max(result, baseColor * 0.3);
            
            FragColor = vec4(result, 1.0);
        }
    )";
    
    if (!m_floorShader.LoadFromSource(floorVertexShader, floorFragmentShader)) {
        return false;
    }
    
    // Sky shader
    const char* skyVertexShader = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 WorldPos;
        
        void main() {
            // Use the position as direction (skybox is centered at origin)
            WorldPos = aPos;
            // Transform to clip space
            vec4 pos = projection * view * vec4(aPos, 1.0);
            gl_Position = pos.xyww; // Set depth to 1.0 (furthest) so sky is always behind
        }
    )";
    
    const char* skyFragmentShader = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 WorldPos;
        
        void main() {
            // Create a gradient sky from dark blue at top to lighter at horizon
            vec3 topColor = vec3(0.2, 0.4, 0.8);    // Dark blue
            vec3 bottomColor = vec3(0.6, 0.7, 0.9); // Light blue-gray
            
            // Normalize WorldPos to get direction
            vec3 dir = normalize(WorldPos);
            
            // Use Y coordinate to blend (dir.y ranges from -1 to 1)
            float t = (dir.y + 1.0) * 0.5; // Normalize from [-1, 1] to [0, 1]
            t = smoothstep(0.0, 1.0, t); // Smooth transition
            
            vec3 color = mix(bottomColor, topColor, t);
            
            // Add some atmospheric haze near horizon
            float horizon = abs(dir.y);
            if (horizon < 0.2) {
                float hazeAmount = (0.2 - horizon) / 0.2;
                color = mix(color, vec3(0.8, 0.8, 0.9), hazeAmount * 0.3);
            }
            
            FragColor = vec4(color, 1.0);
        }
    )";
    
    if (!m_skyShader.LoadFromSource(skyVertexShader, skyFragmentShader)) {
        return false;
    }
    
    SetupWallRendering();
    SetupFloorRendering();
    SetupSkyRendering();
    
    m_initialized = true;
    return true;
}

void Renderer::Shutdown() {
    if (m_wallVAO != 0) {
        glDeleteVertexArrays(1, &m_wallVAO);
        glDeleteBuffers(1, &m_wallVBO);
        glDeleteBuffers(1, &m_wallEBO);  // Delete once at cleanup
    }
    if (m_floorVAO != 0) {
        glDeleteVertexArrays(1, &m_floorVAO);
        glDeleteBuffers(1, &m_floorVBO);
        glDeleteBuffers(1, &m_floorEBO);
    }
    if (m_skyVAO != 0) {
        glDeleteVertexArrays(1, &m_skyVAO);
        glDeleteBuffers(1, &m_skyVBO);
        glDeleteBuffers(1, &m_skyEBO);
    }
}

void Renderer::SetupWallRendering() {
    glGenVertexArrays(1, &m_wallVAO);
    glGenBuffers(1, &m_wallVBO);
    glGenBuffers(1, &m_wallEBO);  // Create once
    
    glBindVertexArray(m_wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_wallVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallEBO);  // Bind to VAO
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Renderer::SetupFloorRendering() {
    glGenVertexArrays(1, &m_floorVAO);
    glGenBuffers(1, &m_floorVBO);
    glGenBuffers(1, &m_floorEBO);
    
    glBindVertexArray(m_floorVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorEBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void Renderer::SetupSkyRendering() {
    // Create a large cube for the skybox
    float skySize = 100.0f;
    float skyVertices[] = {
        // Position
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
        // Front face (inverted winding for inside view)
        0, 3, 2, 2, 1, 0,
        // Back face (inverted winding)
        4, 5, 6, 6, 7, 4,
        // Top face (inverted winding)
        3, 7, 6, 6, 2, 3,
        // Skip bottom face - don't render it to avoid covering floor
        // Right face (inverted winding)
        1, 2, 6, 6, 5, 1,
        // Left face (inverted winding)
        0, 4, 7, 7, 3, 0,
    };
    
    glGenVertexArrays(1, &m_skyVAO);
    glGenBuffers(1, &m_skyVBO);
    glGenBuffers(1, &m_skyEBO);
    
    glBindVertexArray(m_skyVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyIndices), skyIndices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Renderer::RenderLevel(const Level& level, const Camera& camera, float aspectRatio) {
    // Render sky first (background) - with depth test disabled so everything else covers it
    RenderSky(camera, aspectRatio);
    
    // Re-enable depth testing for floor and walls (sky disabled it)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    
    // Render floor
    m_floorShader.Use();
    
    const auto& floorVertices = level.GetFloorVertices();
    const auto& floorIndices = level.GetFloorIndices();
    
    glBindVertexArray(m_floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
    glBufferData(GL_ARRAY_BUFFER, floorVertices.size() * sizeof(float), floorVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, floorIndices.size() * sizeof(unsigned int), floorIndices.data(), GL_STATIC_DRAW);
    
    // Disable culling for floor (floor is viewed from above, need both sides visible)
    GLboolean cullFaceEnabled;
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
    glDisable(GL_CULL_FACE);
    
    // Floor model matrix - identity (no transformation needed, floor is already in world space)
    glm::mat4 model = glm::mat4(1.0f);
    m_floorShader.SetMat4("model", model);
    m_floorShader.SetMat4("view", camera.GetViewMatrix());
    m_floorShader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_floorShader.SetVec3("lightPos", glm::vec3(0.0f, 8.0f, 5.0f));
    m_floorShader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_floorShader.SetFloat("shadowIntensity", 0.8f);
    
    glDrawElements(GL_TRIANGLES, floorIndices.size(), GL_UNSIGNED_INT, 0);
    
    // Restore culling state
    if (cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    }
    
    // Render walls
    m_wallShader.Use();
    m_wallShader.SetMat4("view", camera.GetViewMatrix());
    m_wallShader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    m_wallShader.SetVec3("lightPos", glm::vec3(0.0f, 8.0f, 5.0f));
    m_wallShader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_wallShader.SetVec3("viewPos", camera.GetPosition());
    m_wallShader.SetFloat("shadowIntensity", 0.8f); // 0.0 = no shadows, 1.0 = full shadows
    
    glBindVertexArray(m_wallVAO);
    
    for (const auto& wall : level.GetWalls()) {
        RenderWall(wall, camera);
    }
    
    glBindVertexArray(0);
}

void Renderer::RenderWall(const Wall& wall, const Camera& camera) {
    glm::vec3 wallDir = wall.end - wall.start;
    glm::vec3 wallNormal = glm::normalize(glm::vec3(-wallDir.z, 0.0f, wallDir.x));
    
    float vertices[] = {
        // Position              // Normal
        wall.start.x, 0.0f, wall.start.z,  wallNormal.x, wallNormal.y, wallNormal.z,
        wall.end.x, 0.0f, wall.end.z,      wallNormal.x, wallNormal.y, wallNormal.z,
        wall.end.x, wall.height, wall.end.z, wallNormal.x, wallNormal.y, wallNormal.z,
        wall.start.x, wall.height, wall.start.z, wallNormal.x, wallNormal.y, wallNormal.z,
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, m_wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallEBO);  // Reuse existing
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glm::mat4 model = glm::mat4(1.0f);
    m_wallShader.SetMat4("model", model);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::RenderPlayer(const Player& player, const Camera& camera) {
    // Player rendering can be added here (weapon, crosshair, etc.)
    // For now, this is a placeholder
}

void Renderer::RenderSky(const Camera& camera, float aspectRatio) {
    // Save current state
    GLboolean depthMaskEnabled;
    GLint depthFuncInt;
    GLboolean cullFaceEnabled;
    GLboolean depthTestEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    glGetIntegerv(GL_DEPTH_FUNC, &depthFuncInt);
    glGetBooleanv(GL_CULL_FACE, &cullFaceEnabled);
    glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled);
    
    // Setup for sky rendering - sky should always be behind everything
    // Enable depth test with LEQUAL so sky only draws where depth is at max (1.0)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // Draw if depth is less than or equal (sky is at max depth)
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glDisable(GL_CULL_FACE); // Draw both sides of sky faces
    
    m_skyShader.Use();
    
    // Remove translation from view matrix so sky stays centered on camera
    glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    m_skyShader.SetMat4("view", view);
    m_skyShader.SetMat4("projection", camera.GetProjectionMatrix(aspectRatio));
    
    glBindVertexArray(m_skyVAO);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0); // 5 faces * 2 triangles * 3 vertices (no bottom face)
    
    // Restore state
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    glDepthMask(depthMaskEnabled);
    glDepthFunc(static_cast<GLenum>(depthFuncInt)); // Cast GLint to GLenum explicitly
    if (cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    }
}

