#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Direction.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Mode.h"



class PlayerObject : public GameObject {

    public:

        Direction CurrentDirection;
        Direction QueuedDirection;
        Direction Facing;
        int Row;
        int Col;
        float FreezeFrameTimer = 0.0f;
        float EatTimer = 0.0f;
        PlayerMode CurrentMode;
        PlayerObject();
        PlayerObject(glm::vec2 pos, glm::vec2 size, const float tileSize, float velocity, std::vector<Texture2D> sprites, float cycleLength, std::string name);

        void EnterEatingState();
        void ExitEatingState();
        void Update(float dt, float screenWidth);
        glm::vec2 Move(float dt, float screenWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif 