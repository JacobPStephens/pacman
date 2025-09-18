#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include "GameObject.h"
#include "Texture2D.h"


class PlayerObject : public GameObject {

    public:

        float Speed;
        glm::vec2 Direction;
        PlayerObject();
        PlayerObject(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, float cycleLength);

        void Update(float dt);
        glm::vec2 Move(float dt);
        void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif 