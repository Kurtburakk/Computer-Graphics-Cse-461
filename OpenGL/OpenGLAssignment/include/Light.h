#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

enum class LightType { Directional = 0, Point = 1, Spot = 2 };

class Light {
public:
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(
        LightType type,
        glm::vec3 position = glm::vec3(0.0f),
        glm::vec3 direction = glm::vec3(0.0f),
        glm::vec3 ambient = glm::vec3(0.05f),
        glm::vec3 diffuse = glm::vec3(0.8f),
        glm::vec3 specular = glm::vec3(1.0f)
    ) : type(type), position(position), direction(direction),
        ambient(ambient), diffuse(diffuse), specular(specular) {}
};

#endif