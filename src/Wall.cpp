#include "Wall.h"


Wall::Wall(glm::vec2 pos, glm::vec2 size, std::vector<Texture2D> sprites, int id)
    : GameObject(pos, size, sprites, 1.0f, glm::vec3(1.0f), glm::vec2(0.0f), std::string ("wall" + std::to_string(id)))
{
 
};
