#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Material {
    std::string texture1Path;
    std::string texture2Path;
    float blendFactor;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Object {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationAngle;
    bool isAnimated;

    Material material;
    unsigned int texture1;
    unsigned int texture2;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    Object(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
           const Material& material,
           glm::vec3 pos, glm::vec3 scale, glm::vec3 rotAxis, float rotAngle, bool animated = false);

    void Draw(Shader &shader);
    void Update(float deltaTime);

    glm::vec3 getMinAABB() const;
    glm::vec3 getMaxAABB() const;

    ~Object();
private:
    unsigned int createWhiteTexture();
    void setupMesh();
    void loadTextures();
};

#endif