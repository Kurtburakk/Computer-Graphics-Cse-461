#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Model {
public:
    std::vector<Mesh> meshes;
    Model(const std::string& path);
    void Draw(Shader& shader) const;
private:
    void loadModel(const std::string& path);
    void processNode(struct aiNode* node, const struct aiScene* scene, const std::string& directory);
    Mesh processMesh(struct aiMesh* mesh, const struct aiScene* scene, const std::string& directory);
};

#endif