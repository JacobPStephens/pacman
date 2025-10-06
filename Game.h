#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <vector>
#include <map>
#include <tuple>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameLevel.h"
#include "PlayerObject.h"
#include "Direction.h"


enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;
class Game {

    public:

        GameState State;
        

        unsigned int Width, Height;
        bool Keys[1024];

        Game(unsigned int width, unsigned int height);
        ~Game();

        // initialize shaders, textures, levels
        void Init();

        void BuildWalls();
        void BuildDots();
        void BuildTiles();

        std::vector<std::vector<int>>* BuildMap();

        std::vector<std::string> Split(std::string);
        void PrintVector(std::vector<std::string>);
        void Win();

        // core game loop
        void ProcessInput(float dt);
        void Update(float dt);

        GameObject* GhostMapTarget;

        std::tuple<glm::vec2, glm::vec2> AssembleWallInfo(float x1, float y1, float x2, float y2);
        void HandleCollisions();
        void HandleDotCollisions();

        bool CheckPlayerWallCollision(Direction direction);
        void SnapPlayerToGrid(Direction direction, bool bothAxes);
        bool CheckCollision(glm::vec2 onePos, glm::vec2 oneSize, glm::vec2 twoPos, glm::vec2 twoSize);
        void Render();


};

#endif