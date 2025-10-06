#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Direction.h"
#include "GameObject.h"
#include "Texture2D.h"


class PlayerObject : public GameObject {

    public:

        Direction CurrentDirection;
        Direction QueuedDirection;
        int Row;
        int Col;
        PlayerObject();
        PlayerObject(glm::vec2 pos, glm::vec2 size, const float tileSize, glm::vec2 velocity, std::vector<Texture2D> sprites, float cycleLength, std::string name);

        void Update(float dt, float screenWidth);
        glm::vec2 Move(float dt, float screenWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif 