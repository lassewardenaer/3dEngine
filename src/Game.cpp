#include "Game.h"
#include <iostream>
#include <cmath>
#include "OsUtils.h"

Game::Game() 
    : m_window(nullptr)
    , m_width(0)  // Default size, will be set from screen size
    , m_height(0)
    , m_lastX(m_width / 2.0f)
    , m_lastY(m_height / 2.0f)
    , m_firstMouse(true)
    , m_lastFrame(0.0f)
{
    // Get screen size and use 80% of it for window size
    WindowSize screenSize = OsUtils::getSizeOfWindow();
    m_width = static_cast<int>(screenSize.width);
    m_height = static_cast<int>(screenSize.height);
    
    // Update lastX and lastY based on actual window size
    m_lastX = m_width / 2.0f;
    m_lastY = m_height / 2.0f;
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    m_window = glfwCreateWindow(m_width, m_height, "Doom-Like Game", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    // Initialize renderer
    if (!m_renderer.Initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return false;
    }
    
    // Initialize level
    if (!m_level.Initialize()) {
        std::cerr << "Failed to initialize level" << std::endl;
        return false;
    }
    
    // Initialize player at a safe position (not on a wall)
    // Avoid z=0 (horizontal wall) and x=±5 (vertical walls)
    m_player.SetPosition(glm::vec3(0.0f, 0.5f, 2.0f));
    
    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    std::cout << "Game initialized successfully!" << std::endl;
    return true;
}

void Game::Run() {
    while (!glfwWindowShouldClose(m_window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
        
        ProcessInput(deltaTime);
        Update(deltaTime);
        Render();
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Game::ProcessInput(const float& deltaTime) {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    const float collisionRadius = 0.5f;
    glm::vec3 currentPos = m_player.GetPosition();
    
    // Player movement - check collision before each movement
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        // Calculate where we would move to
        float yawRad = glm::radians(m_player.GetYaw());
        glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
        glm::vec3 newPos = currentPos + direction * m_player.GetSpeed() * deltaTime;
        
        // Only move if new position doesn't collide
        if (!m_level.CheckCollision(newPos, collisionRadius)) {
            m_player.MoveForward(deltaTime);
        }
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        float yawRad = glm::radians(m_player.GetYaw());
        glm::vec3 direction(cos(yawRad), 0.0f, sin(yawRad));
        glm::vec3 newPos = currentPos - direction * m_player.GetSpeed() * deltaTime;
        
        if (!m_level.CheckCollision(newPos, collisionRadius)) {
            m_player.MoveBackward(deltaTime);
        }
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
        float yawRad = glm::radians(m_player.GetYaw());
        glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
        glm::vec3 newPos = currentPos - direction * m_player.GetSpeed() * deltaTime;
        
        if (!m_level.CheckCollision(newPos, collisionRadius)) {
            m_player.MoveLeft(deltaTime);
        }
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
        float yawRad = glm::radians(m_player.GetYaw());
        glm::vec3 direction(-sin(yawRad), 0.0f, cos(yawRad));
        glm::vec3 newPos = currentPos + direction * m_player.GetSpeed() * deltaTime;
        
        if (!m_level.CheckCollision(newPos, collisionRadius)) {
            m_player.MoveRight(deltaTime);
        }
    }
    
    // Shooting
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_player.Shoot(deltaTime);
    }
}

void Game::Update(const float& deltaTime) {
    m_player.Update(deltaTime);
    
    m_camera.SetPosition(m_player.GetPosition());
    m_camera.SetYaw(m_player.GetYaw());
    m_camera.SetPitch(m_player.GetPitch());
}

void Game::Render() {
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f); // Light blue background (sky color fallback)
    glClearDepth(1.0f); // Clear depth buffer to 1.0 (max depth)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Depth testing will be managed by renderer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    
    float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_renderer.RenderLevel(m_level, m_camera, aspectRatio);
    m_renderer.RenderPlayer(m_player, m_camera, aspectRatio);
}

void Game::Shutdown() {
    m_renderer.Shutdown();
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->m_width = width;
    game->m_height = height;
    glViewport(0, 0, width, height);
}

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    
    if (game->m_firstMouse) {
        game->m_lastX = xpos;
        game->m_lastY = ypos;
        game->m_firstMouse = false;
    }
    
    float xoffset = xpos - game->m_lastX;
    float yoffset = game->m_lastY - ypos;
    
    game->m_lastX = xpos;
    game->m_lastY = ypos;
    
    game->m_player.ProcessMouseMovement(xoffset, yoffset);
}

