#ifndef WALL_CLASS_H
#define WALL_CLASS_H

#include <glm/vec2.hpp>
#include <vector>
#include "GameObject.h"

class Wall : public GameObject {

    public:
        Wall(glm::vec2 pos, glm::vec2 size, std::vector<Texture2D> sprites, int id);
        
};

#endif