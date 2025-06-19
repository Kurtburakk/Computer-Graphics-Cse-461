#include "World.h"
#include <algorithm>

static std::vector<float> cubeVertices = {
    -0.5,-0.5,-0.5,   0,0,-1,  0,0,
     0.5,-0.5,-0.5,   0,0,-1,  1,0,
     0.5, 0.5,-0.5,   0,0,-1,  1,1,
    -0.5, 0.5,-0.5,   0,0,-1,  0,1,

    -0.5,-0.5, 0.5,   0,0,1,   0,0,
     0.5,-0.5, 0.5,   0,0,1,   1,0,
     0.5, 0.5, 0.5,   0,0,1,   1,1,
    -0.5, 0.5, 0.5,   0,0,1,   0,1,

    -0.5, 0.5, 0.5,  -1,0,0,   1,0,
    -0.5, 0.5,-0.5,  -1,0,0,   1,1,
    -0.5,-0.5,-0.5,  -1,0,0,   0,1,
    -0.5,-0.5, 0.5,  -1,0,0,   0,0,

     0.5, 0.5, 0.5,   1,0,0,   1,0,
     0.5, 0.5,-0.5,   1,0,0,   1,1,
     0.5,-0.5,-0.5,   1,0,0,   0,1,
     0.5,-0.5, 0.5,   1,0,0,   0,0,

    -0.5,-0.5,-0.5,   0,-1,0,  0,1,
     0.5,-0.5,-0.5,   0,-1,0,  1,1,
     0.5,-0.5, 0.5,   0,-1,0,  1,0,
    -0.5,-0.5, 0.5,   0,-1,0,  0,0,

    -0.5, 0.5,-0.5,   0,1,0,   0,1,
     0.5, 0.5,-0.5,   0,1,0,   1,1,
     0.5, 0.5, 0.5,   0,1,0,   1,0,
    -0.5, 0.5, 0.5,   0,1,0,   0,0,
};

static std::vector<unsigned int> cubeIndices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

World::World()
    : minBounds(glm::vec3(-25.0f, 0.0f, -25.0f)), maxBounds(glm::vec3(15.0f, 4.0f, 15.0f)) // sınırları belirliyorum
{
}

std::vector<Object> &World::createSceneObjects()
{
    Material mat1 = {"resources/textures/brick.jpg", "", 0.0f,
                     glm::vec3(0.2f), glm::vec3(0.7f), glm::vec3(0.5f), 32.0f};
    Material mat2 = {"resources/textures/wood.jpg", "", 0.0f,
                     glm::vec3(0.2f), glm::vec3(0.6f), glm::vec3(0.3f), 16.0f};
    Material mat3 = {"resources/textures/metal.jpg", "resources/textures/rust.jpg", 0.5f,
                     glm::vec3(0.2f), glm::vec3(0.6f), glm::vec3(0.8f), 64.0f};
    Material mat4 = {"resources/textures/marble.jpg", "", 0.0f,
                     glm::vec3(0.3f), glm::vec3(0.8f), glm::vec3(0.5f), 8.0f};

    // Floor
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat1, glm::vec3(0, 0, 0), glm::vec3(50.0f, 0.1f, 50.0f), glm::vec3(0, 1, 0), 0.0f);
    
    // animasyonlu küp olacak
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat3, glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec3(0, 1, 0), 0.0f, true);

    // uzun küp
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat4, glm::vec3(-2, 1, 2), glm::vec3(0.6f, 2.0f, 0.6f), glm::vec3(0, 1, 0), 0.0f);

    // küçük küpler
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat3, glm::vec3(-1, 0.5f, -2), glm::vec3(0.5f), glm::vec3(1, 0, 0), 0.0f);
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat3, glm::vec3(2, 0.5f, 1), glm::vec3(0.7f, 0.5f, 0.4f), glm::vec3(0, 1, 0), 0.0f);
    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat3, glm::vec3(3, 0.5f, -3), glm::vec3(0.8f), glm::vec3(0, 0, 1), 0.0f);

    sceneObjects.emplace_back(cubeVertices, cubeIndices, mat4, glm::vec3(0, 0.5f, 3), glm::vec3(1.2f, 0.5f, 1.2f), glm::vec3(0, 0, 1), 0.0f);

    return sceneObjects;
}

bool World::checkCollision(const glm::vec3 &minAABB, const glm::vec3 &maxAABB) const
{
    if (minAABB.x < minBounds.x || maxAABB.x > maxBounds.x)
        return true;
    if (minAABB.z < minBounds.z || maxAABB.z > maxBounds.z)
        return true;

    if (minAABB.y < minBounds.y) // Zemin altına inmesin
        return true;

    for (size_t idx = 0; idx < sceneObjects.size(); ++idx) {
        const auto& obj = sceneObjects[idx];
        glm::vec3 omin = obj.getMinAABB();
        glm::vec3 omax = obj.getMaxAABB();
        bool overlap = (minAABB.x <= omax.x && maxAABB.x >= omin.x) &&
                       (minAABB.y <= omax.y && maxAABB.y >= omin.y) &&
                       (minAABB.z <= omax.z && maxAABB.z >= omin.z);
        if (overlap)
            return true;
    }
    return false;
}