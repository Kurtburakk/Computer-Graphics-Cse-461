#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include <iostream>

Input::Input(Camera& cameraRef, World& worldRef, float& deltaTimeRef)
    : camera(cameraRef), world(worldRef), deltaTime(deltaTimeRef), lastX(640.0f), lastY(360.0f), firstMouse(true)
{ }

void Input::processInput(GLFWwindow* window)
{
    glm::vec3 prevPos = camera.Position;
    float moveStep = deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, moveStep);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, moveStep);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, moveStep);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, moveStep);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, moveStep);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, moveStep);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, moveStep);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, moveStep);

    glm::vec3 camMin = camera.Position - glm::vec3(0.2f, 0.8f, 0.2f);
    glm::vec3 camMax = camera.Position + glm::vec3(0.2f, 0.8f, 0.2f);

    if (world.checkCollision(camMin, camMax)) {
        camera.Position = prevPos;
    }
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float fx = static_cast<float>(xpos);
    float fy = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = fx;
        lastY = fy;
        firstMouse = false;
    }

    float xoffset = fx - lastX;
    float yoffset = lastY - fy;
    lastX = fx;
    lastY = fy;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Input::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}