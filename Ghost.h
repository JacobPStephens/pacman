#ifndef GHOST_CLASS_H
#define GHOST_CLASS_H

#include "Direction.h"
#include "Mode.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Eyes.h"
#include "ResourceManager.h"

#include <array>

class Ghost : public GameObject {

    public:

        Ghost(std::string name, GameObject* Player, std::vector<std::vector<int>>* map, const float tileSize, Ghost* redGhost=nullptr);
        Ghost(glm::vec2 pos, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, glm::vec3 color, float alpha, float rotation, std::string name, float tileSize, PlayerObject* player, std::vector<std::vector<int>> *map, GameObject* ghostMapTarget, GameObject* ghostAdjTarget);
        static glm::vec3 GetGhostRate(std::string name);
        static std::array<float, 8> GetScatterTimings(int level);

        // Mode switching
        Mode CurrentMode = SCATTER;
        static std::array<float, 8> ScatterTimings;
        int ModeIdx = 0;
        float ModeTimer;

        ~Ghost();
        glm::vec2 targetPos;
        float TILE_SIZE;

        std::vector<std::vector<int>>* Map;

        // int Row;
        // int Col;

        static std::map<std::string, glm::vec2> ScatterTile;

        static int EXAMPLE_INT;

        GameObject* ghostMapTarget;
        GameObject* ghostAdjTarget;

        GameObject* DebugMapTarget;
        GameObject* DebugAdjTarget;

        Eyes* EyesObj;


        glm::vec2 AdjTargetPos;
        glm::vec2 MapTargetPos;

        Direction Dir = RIGHT;

        void Update(float deltaTime, PlayerObject* player, Ghost* redGhost=nullptr);
        void SnapToGrid(Direction direction, bool bothAxes);
        glm::vec2 Move(float deltaTime, glm::vec2 ghostPos, Direction dir, float speed);
        glm::vec2 GetMapTarget(int playerRow, int playerCol, Direction playerDir, std::string ghostType, Mode currentMode, Ghost* redGhost=nullptr);
        glm::vec2 GetAdjTarget(int row, int col, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, float tileSize, std::vector<std::vector<int>>* map);
        Direction GetDirection(int row, int col, glm::vec2 adjTargetPos);

        float GetDistance(float x1, float y1, float x2, float y2);

        bool AdjTargetReached(glm::vec2 ghostPos, glm::vec2 adjTargetPos, float tileSize);

        void TurnAround(Direction currentDir, glm::vec2 currentAdjTarget);

        

};

#endif