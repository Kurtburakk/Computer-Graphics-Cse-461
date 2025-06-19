#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "World.h"
#include "Light.h"
#include "Input.h"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(2.0f, 2.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
World world;
std::vector<Object> objects;
std::vector<Light> lights;
Input *input = nullptr;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (input)
        input->mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (input)
        input->scroll_callback(window, xoffset, yoffset);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    if (input)
        input->framebuffer_size_callback(window, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Assignment", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    input = new Input(camera, world, deltaTime);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    Shader shaders("shaders/phong.vert", "shaders/phong.frag");
    objects = world.createSceneObjects();

    lights.emplace_back(LightType::Directional,
                        glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, -0.0f),
                        glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    lights.emplace_back(LightType::Point,
                        glm::vec3(2.0f, 3.0f, 1.0f), glm::vec3(0.0f),
                        glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    lights.emplace_back(LightType::Point,
                        glm::vec3(10.0f, 8.0f, -10.0f), glm::vec3(0.0f),
                        glm::vec3(0.05f), glm::vec3(1.0f, 0.8f, 0.6f), glm::vec3(1.0f, 0.8f, 0.6f));

    lights.emplace_back(LightType::Point,
                        glm::vec3(-10.0f, 2.0f, 10.0f), glm::vec3(0.0f),
                        glm::vec3(0.05f), glm::vec3(0.6f, 0.8f, 1.0f), glm::vec3(0.6f, 0.8f, 1.0f));

    // Otomatik yüklemeye geçtim
    std::vector<Model> loadedModels;
    std::string modelDir = "resources/models/";
    for (const auto &entry : std::filesystem::directory_iterator(modelDir))
    {
        if (entry.path().extension() == ".obj")
        {
            loadedModels.emplace_back(entry.path().string());
        }
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input->processInput(window);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shaders.use();
        shaders.setMat4("projection", projection);
        shaders.setMat4("view", view);

        for (size_t i = 0; i < lights.size(); ++i)
        {
            std::string idx = "lights[" + std::to_string(i) + "]";
            shaders.setVec3(idx + ".position", lights[i].position);
            shaders.setVec3(idx + ".direction", lights[i].direction);
            shaders.setVec3(idx + ".ambient", lights[i].ambient);
            shaders.setVec3(idx + ".diffuse", lights[i].diffuse);
            shaders.setVec3(idx + ".specular", lights[i].specular);
            shaders.setInt(idx + ".type", static_cast<int>(lights[i].type));
        }
        shaders.setInt("numLights", static_cast<int>(lights.size()));
        shaders.setVec3("viewPos", camera.Position);

        // Otomatik yüklenen tüm modelleri loop a alıp çiz bazılarının scale ini değiş
        for (size_t i = 0; i < loadedModels.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            // Modelleri farklı farklı alanlara belirli mesafe ile dağıtıyorum
            if (i % 2 == 0)
            {
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 6 * (i + 1) * 1.0f));
                model = glm::scale(model, glm::vec3(2.0f));
            }
            else
            {
                model = glm::translate(model, glm::vec3(7 * (i + 1) * 1.0f, 0.0f, 2.0f));
                model = glm::scale(model, glm::vec3(0.5f));
            }

            
            shaders.setMat4("model", model);
            loadedModels[i].Draw(shaders);
        }

        for (auto &obj : objects)
        {
            obj.Update(deltaTime);
            obj.Draw(shaders);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete input;
    glfwTerminate();
    return 0;
}