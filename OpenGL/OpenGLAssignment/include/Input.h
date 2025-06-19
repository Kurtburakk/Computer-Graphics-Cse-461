#pragma once
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "World.h"

class Input {
public:
    Input(Camera& camera, World& world, float& deltaTime);

    void processInput(GLFWwindow* window);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    Camera& camera;
    World& world;
    float& deltaTime;
    float lastX, lastY;
    bool firstMouse;
};