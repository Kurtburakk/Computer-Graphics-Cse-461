#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

class World {
public:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    std::vector<Object> sceneObjects;

    World();

    std::vector<Object>& createSceneObjects();

    bool checkCollision(const glm::vec3& minAABB, const glm::vec3& maxAABB) const;
};

#endif