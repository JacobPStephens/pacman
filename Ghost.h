#ifndef GHOST_CLASS_H
#define GHOST_CLASS_H

#include "Direction.h"
#include "GameObject.h"
#include "PlayerObject.h"

class Ghost : public GameObject {

    public:
        Ghost(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, float rotation, std::string name, float tileSize, PlayerObject* player, std::vector<std::vector<int>> *map, GameObject* ghostMapTarget, GameObject* ghostAdjTarget);
        ~Ghost();
        glm::vec2 targetPos;
        float Speed = ((39.75f * tileSize) / 8.0f);
        float TILE_SIZE;

        std::vector<std::vector<int>>* Map;

        // int Row;
        // int Col;

        GameObject* ghostMapTarget;
        GameObject* ghostAdjTarget;


        glm::vec2 AdjTargetPos;
        glm::vec2 MapTargetPos;

        Direction Dir = RIGHT;

        void Update(float deltaTime, PlayerObject* player);
        void SnapToGrid(Direction direction, bool bothAxes);
        glm::vec2 Move(float deltaTime, glm::vec2 ghostPos, Direction dir, float speed);
        glm::vec2 GetAdjTarget(glm::vec2 ghostPos, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, float tileSize, std::vector<std::vector<int>>* map);
        Direction GetDirection(glm::vec2 ghostPos, glm::vec2 adjTargetPos);

        float GetDistance(float x1, float y1, float x2, float y2);

        bool AdjTargetReached(glm::vec2 ghostPos, glm::vec2 adjTargetPos);

};

#endif