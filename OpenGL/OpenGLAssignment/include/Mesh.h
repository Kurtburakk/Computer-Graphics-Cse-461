#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;
    unsigned int textureID;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int textureID);
    void Draw() const;
private:
    unsigned int VBO, EBO;
    void setupMesh();
};

#endif