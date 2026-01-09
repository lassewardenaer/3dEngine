#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "level/Level.h"
#include "gameNodes/characters/Player.h"
#include "renderer/Renderer.h"

class Game {
public:
    Game();
    ~Game();
    
    bool Initialize();
    void Run();
    void ProcessInput(const float& deltaTime);
    void Update(const float& deltaTime);
    void Render();
    void Shutdown();
    
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    
private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    
    Camera m_camera;
    Level m_level;
    Player m_player;
    Renderer m_renderer;
    
    float m_lastX;
    float m_lastY;
    bool m_firstMouse;
    float m_lastFrame;
};

#endif

