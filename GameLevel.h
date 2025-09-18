#ifndef CLASS_GAMELEVEL_H
#define CLASS_GAMELEVEL_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class GameLevel {

    public:
        std::vector<GameObject> Bricks;
        GameLevel() {};
        // load level from file
        void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
        // render level
        void Draw(SpriteRenderer &renderer);
        // check for level completion
        bool IsCompleted();
    private:
        // initialize level from tile data
        void init(std::vector<std::vector<unsigned int>> tiles, unsigned int levelWidth, unsigned int levelHeight);

};

#endif