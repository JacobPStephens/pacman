#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Ghost.h"
#include "Eyes.h"

#include "Wall.h"
#include "LevelSpecs.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>

#include <filesystem>

SpriteRenderer* Renderer;
std::queue<float> fpsQueue;
float queueSum;


const glm::vec2 PLAYER_SIZE(13.0f/8.0f * TILE_SIZE);
const glm::vec2 GHOST_SIZE(14.0f/8.0f * TILE_SIZE);
//const float PLAYER_VELOCITY(53.0f/8.0f * TILE_SIZE);
const float PLAYER_VELOCITY(speedRatio * TILE_SIZE * LevelSpecs["playerSpeed"][1]);

int remainingDots = 240 + 4;


// 0.129, 0.129, 1.0

// 256.074 RED GHOST SPAWN Y
PlayerObject* Player;
//Ghost* NewGhost;

std::vector<Ghost*> Ghosts;
// Ghost* RedGhost;
// Eyes* RedGhostEyes;
// GameObject* RedGhostMapTarget;
// GameObject* RedGhostAdjTarget;
// Ghost* PinkGhost;
// Eyes* PinkGhostEyes;
// GameObject* PinkGhostMapTarget;
// GameObject* PinkGhostAdjTarget;

//GameObject* Wall;
//GameObject* Wall2;
GameObject* Background;
GameObject* CurrentSensor;
GameObject* QueuedSensor;
std::vector<GameObject*> walls;
std::vector<GameObject*> dots;
std::vector<GameObject*> powerDots;

GameObject* MapTiles;
std::vector<std::vector<int>>* map;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{

}

Game::~Game() {
    delete Renderer;
    delete Player;
    delete map;
    // delete RedGhost;
    // delete RedGhostEyes;
    // delete RedGhostMapTarget;
    // delete RedGhostAdjTarget;
    // delete PinkGhost;
    // delete PinkGhostEyes;
    // delete PinkGhostMapTarget;
    // delete PinkGhostAdjTarget;
    //delete NewGhost;

    for (Ghost* ghost : Ghosts) {
        delete ghost;
    }
    //delete Wall;
    //delete Wall2;
    delete CurrentSensor;
    delete QueuedSensor;
    delete Background;

    // delete all wall ptrs
    for (int i = 0; i < walls.size(); i++) {
        delete walls[i];
    }
    for (int i = 0; i < dots.size(); i++) {
        delete dots[i];
    }  
    for (int i = 0; i < powerDots.size(); i++) {
        delete powerDots[i];
    }      
    delete MapTiles;
}


// const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// const float BALL_RADIUS = 12.5f;
// BallObject *Ball;

std::vector<std::vector<int>>* Game::BuildMap() {
    std::ifstream mapInput;
    std::string line;
    
    mapInput.open("/home/jacob/source/pacman/util/map.txt");
    if (!mapInput.is_open())
        std::cout <<"MAP FILE DID NOT OPEN" << std::endl;

    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;

    map = new std::vector<std::vector<int>>();
    
    while (std::getline (mapInput, line)) {
        std::vector<int> rowInt;
        std::vector<std::string> rowStr = Split(line);

        //PrintVector(rowStr);
        for (int i = 0; i < rowStr.size(); i++) {
            //std::cout << rowStr[i] << std::endl;
            //std::cout << rowStr[i].size() << std::endl;

            if (rowStr[i] == "") {
                std::cout <<" Empty string" << std::endl;
            }
            // if (rowStr[i][0] == '\r') {
            //     std::cout << "new line detected";
            // }

            rowInt.push_back(std::stoi(rowStr[i]));
        }  

        map->push_back(rowInt);
        



        //PrintVector(coordinates);
        //glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        //glm::vec2 dotPos(tilePos.x + halfTile - halfDot, tilePos.y + halfTile - halfDot);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;

    }
    mapInput.close();
    return map;
}
void Game::BuildDots() {

    std::ifstream dotInput;
    std::string line;
    
    dotInput.open("/home/jacob/source/pacman/util/dots.txt");
    if (!dotInput.is_open())
        std::cout <<"DOT FILE DID NOT OPEN" << std::endl;


    // for (int i = 0; i < ans.size(); i++) {
    //     std::cout<<ans[i]<<std::endl;
    // }
    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;
    std::vector<Texture2D> pxSprite = { ResourceManager::GetTexture("px")};
    while (std::getline (dotInput, line)) {
        std::vector<std::string> coordinates = Split(line);
        //PrintVector(coordinates);
        glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        glm::vec2 dotPos(tilePos.x + halfTile - halfDot, tilePos.y + halfTile - halfDot);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;
        GameObject* d = new GameObject(dotPos, glm::vec2(TILE_SIZE / 4.0f), TILE_SIZE, pxSprite);
        dots.push_back(d);

    }
    dotInput.close();
}

void Game::BuildPowerDots() {
    std::cout<<"Building dots..."<<std::endl;
    std::ifstream powerDotInput;
    std::string line;
    
    powerDotInput.open("/home/jacob/source/pacman/util/powerDots.txt");
    if (!powerDotInput.is_open())
        std::cout <<"DOT FILE DID NOT OPEN" << std::endl;


    // for (int i = 0; i < ans.size(); i++) {
    //     std::cout<<ans[i]<<std::endl;
    // }
    float halfTile = TILE_SIZE / 2.0f;
    float halfDot = (TILE_SIZE / 4.0f) / 2.0f;
    std::vector<Texture2D> pxSprite = { ResourceManager::GetTexture("powerDot")};
    while (std::getline (powerDotInput, line)) {
        std::vector<std::string> coordinates = Split(line);
        //PrintVector(coordinates);
        glm::vec2 tilePos(std::stoi(coordinates[1]) * TILE_SIZE, std::stoi(coordinates[0]) * TILE_SIZE);
        //std::cout << "Tile Position " << tilePos.x << " " << tilePos.y << std::endl;
        glm::vec2 dotPos(tilePos.x, tilePos.y);
        //std::cout << "Dot Position " << dotPos.x << " " << dotPos.y << std::endl;
        GameObject* d = new GameObject(dotPos, glm::vec2(TILE_SIZE), TILE_SIZE, pxSprite);
        powerDots.push_back(d);

    }
    powerDotInput.close();
}
void Game::BuildWalls() {
    std::vector<Texture2D> grayWallSprites = { ResourceManager::GetTexture("px")};
    std::string line;
    std::ifstream wallInput;

    // read file into 2d vector of walls
    std::vector<std::vector<int>> wallTiles;
    std::vector<int> wall;
    
    wallInput.open("/home/jacob/source/pacman/util/wallsOut.txt");
    if (!wallInput.is_open())
        std::cout <<"WALL FILE DID NOT OPEN" << std::endl;


    while (std::getline (wallInput, line)) {
        std::string num;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != ' ' && line[i] != '\n')
                num += line[i];
            else {
                wall.push_back(std::stoi(num));
                num = "";
            }
        }
        wall.push_back(std::stoi(num));
        wallTiles.push_back(wall);
        wall.clear();
    }
    wallInput.close();

    // construct all walls in 2D vector
    for (int i = 0; i < wallTiles.size(); i++) {
        glm::vec2 pos(wallTiles[i][0] * TILE_SIZE, wallTiles[i][1] * TILE_SIZE);
        glm::vec2 size(wallTiles[i][2] * TILE_SIZE, wallTiles[i][3] * TILE_SIZE);
        //Wall* w = new Wall(pos, size, grayWallSprites, i);
        GameObject* w = new GameObject(pos, size, TILE_SIZE, grayWallSprites);
        walls.push_back(w);
    }
}

std::vector<std::string> Game::Split(std::string s) {
    std::vector<std::string> res;
    std::string part;
    for (int i = 0; i < s.size(); i++) {

        if (!std::isspace(s[i])) {
            part += s[i];
        }
        else {
            if (part != "") {
                res.push_back(part);
            }
            part = "";
        }
    }
    if (part != "" && !std::isspace(part[0])) {
        res.push_back(part);
    }
    return res;

}
void Game::PrintVector(std::vector<std::string> v) {

    std::cout << "<";
    for (int i = 0; i < v.size(); i++) {
        if (i != v.size()-1) {
            std::cout << v[i] << ", ";
        }
        else {
            std::cout << v[i];
        }
    }
    std::cout<<">\n";
}
void Game::Init() {


    //ResourceManager::LoadTexture("textures/pacmanNew1.png", true, "pacman");
    //ResourceManager::LoadTexture("textures/pacmanNew2.png", true, "pacman2");
    ResourceManager::LoadTexture("textures/pacmanNew1.png", true, "pacman1");
    ResourceManager::LoadTexture("textures/finalPacman2.png", true, "pacman2");
    ResourceManager::LoadTexture("textures/pacmanNew2.png", true, "pacman3");

    ResourceManager::LoadTexture("textures/map.png", false, "walls");
    ResourceManager::LoadTexture("textures/mapTiles.png", true, "mapTiles");

    ResourceManager::LoadTexture("textures/grayWall.png", true, "grayWall");
    ResourceManager::LoadTexture("textures/px.png", true, "px");
    ResourceManager::LoadTexture("textures/map224x248.png", true, "map224x248");
    ResourceManager::LoadTexture("textures/powerDot.png", true, "powerDot");

    ResourceManager::LoadTexture("textures/ghost.png", true, "ghost");
    ResourceManager::LoadTexture("textures/eyesRight.png", true, "eyesRight");
    ResourceManager::LoadTexture("textures/eyesUp.png", true, "eyesUp");
    ResourceManager::LoadTexture("textures/eyesLeft.png", true, "eyesLeft");
    ResourceManager::LoadTexture("textures/eyesDown.png", true, "eyesDown");
    ResourceManager::LoadTexture("textures/eyesFrightened.png", true, "eyesFrightened");
    

    
    std::vector<Texture2D> wallSprites = { ResourceManager::GetTexture("map224x248")};
    std::vector<Texture2D> mapTilesSprite = { ResourceManager::GetTexture("mapTiles")};

    glm::vec2 mapSize = glm::vec2(224.0f * RESOLUTION_SCALE, 248.0f * RESOLUTION_SCALE);
    glm::vec2 mapPos = glm::vec2(this->Width/2.0f - mapSize.x/2.0f, 0.0f);

    Background = new GameObject(mapPos, mapSize, TILE_SIZE, wallSprites, 1.0f, glm::vec3(1.0f), 1.0f, 0.0f, std::string("wall"));
    MapTiles = new GameObject(mapPos, mapSize, TILE_SIZE, mapTilesSprite, 1.0f, glm::vec3(1.0f), 0.25f, 0.0f, std::string("wall"));
    BuildWalls();
    BuildPowerDots();
    BuildDots();


    
    map = BuildMap();
    std::vector<Texture2D> pxSprite = { ResourceManager::GetTexture("px")};

    CurrentSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(0.0f, 1.0f, 1.0f), 1.0f, 0.0f, "upSensor");
    QueuedSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, 0.0f, "upSensor");
    // RedGhostMapTarget = new GameObject(glm::vec2(0.0f), glm::vec2(TILE_SIZE), TILE_SIZE, pxSprite, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, glm::vec2(0.0f), "ghostMapTarget");
    // RedGhostAdjTarget = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(0.0f, 1.0f, 1.0f), 1.0f, glm::vec2(0.0f), "ghostAdjTarget");
    // PinkGhostMapTarget = new GameObject(glm::vec2(0.0f), glm::vec2(TILE_SIZE), TILE_SIZE, pxSprite, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, glm::vec2(0.0f), "ghostMapTarget");
    // PinkGhostAdjTarget = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), TILE_SIZE, pxSprite, 1.0f, glm::vec3(0.0f, 1.0f, 1.0f), 1.0f, glm::vec2(0.0f), "ghostAdjTarget");

    glm::vec2 startingTileCenter = glm::vec2(13.0f * TILE_SIZE + TILE_SIZE/2.0f, 23.0f * TILE_SIZE + TILE_SIZE/2.0f);

    // glm::vec2 playerPos = glm::vec2(
    //     this->Width/2.0f - PLAYER_SIZE.x/2.0f,
    //     552.0f
    //     //544.226f // got by logging pacman's position while in correct row
    // );

    glm::vec2 playerPos = glm::vec2(startingTileCenter.x-PLAYER_SIZE.x/2.0f, startingTileCenter.y-PLAYER_SIZE.y/2.0f);

    std::vector<Texture2D> sprites = {
        ResourceManager::GetTexture("pacman1"),
        ResourceManager::GetTexture("pacman3"),
    };
    Player = new PlayerObject(playerPos, PLAYER_SIZE, TILE_SIZE, PLAYER_VELOCITY, sprites, 0.25f, std::string("player"));

    std::vector<Texture2D> ghostSprite = {
        ResourceManager::GetTexture("ghost")
    };
    std::vector<Texture2D> eyesSprites = {
        ResourceManager::GetTexture("eyesRight"),
        ResourceManager::GetTexture("eyesUp"),
        ResourceManager::GetTexture("eyesLeft"),
        ResourceManager::GetTexture("eyesDown"),
    };

    //NewGhost = new Ghost("red", Player, map, TILE_SIZE);

    std::array<std::string, 4> ghostColors = { "red", "pink", "blue", "orange"};

    for (std::string c : ghostColors) {
        Ghosts.push_back(new Ghost(c , Player, map, TILE_SIZE));
    }
    // Ghosts.push_back(new Ghost("red", Player, map, TILE_SIZE));
    // Ghosts.push_bac
    // RedGhost = new Ghost(playerPos, GHOST_SIZE, glm::vec2(PLAYER_VELOCITY), ghostSprite, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.25f, std::string("redGhost"), TILE_SIZE, Player, map, RedGhostMapTarget, RedGhostAdjTarget);
    // RedGhostEyes = new Eyes(RedGhost, TILE_SIZE, eyesSprites);
    // PinkGhost = new Ghost(playerPos, GHOST_SIZE, glm::vec2(PLAYER_VELOCITY), ghostSprite, glm::vec3(1.0f, 0.72f, 1.0f), 1.0f, 0.25f, std::string("pinkGhost"), TILE_SIZE, Player, map, PinkGhostMapTarget, PinkGhostAdjTarget);
    // PinkGhostEyes = new Eyes(PinkGhost, TILE_SIZE, eyesSprites);
    // load shaders
    ResourceManager::LoadShader("shaders/default.vert", "shaders/default.frag", nullptr, "sprite"); 
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // update uniform variables in shader
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);

    // create renderer
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

}

std::tuple<glm::vec2, glm::vec2> Game::AssembleWallInfo(float x1, float y1, float x2, float y2) {
    float wallWidth = 4.0f;
    bool isHorizontal = (y1 == y2);
    glm::vec2 size;
    if (isHorizontal) {
        size = glm::vec2(x2-x1,wallWidth);
    }
    else {
        size = glm::vec2(wallWidth, y2-y1);
    }
    glm::vec2 pos(x1, y1);

    return std::make_tuple(pos, size);


}



void Game::Update(float dt) {
    // std::cout << "Speed ratio: " << std::endl;
    // std::cout << "TILE size: " << TILE_SIZE << std::endl;
    // std::cout << "player speed ratio" << LevelSpecs["playerSpeed"][1] << std::endl;
    // std::cout << "Total speed: " << PLAYER_VELOCITY << std::endl;
    Player->Update(dt, 224.0f * RESOLUTION_SCALE);
    for (Ghost* ghost : Ghosts) {

        if (ghost->Name == "blueGhost") {
            ghost->Update(dt, Player, Ghosts.at(0));
        }
        else {
            ghost->Update(dt, Player);
        }


    }
    //NewGhost->Update(dt, Player);
    // RedGhost->Update(dt, Player);
    // RedGhostEyes->Update(dt);
    // PinkGhost->Update(dt, Player);
    // PinkGhostEyes->Update(dt);

    //std::cout << "Ghost: " << RedGhost->Position.x << " " << RedGhost->Position.y << std::endl;
    //std::cout << CurrentSensor->Position.x << " " << CurrentSensor->Position.y << std::endl;
    //std::cout << TILE_SIZE * << std::endl;

    //Wall->Update(dt);
    
    HandleCollisions();

    //Player->Move(dt);
    //PrintFPS(dt);
}

void Game::HandleCollisions() {
    std::map<Direction, float> dirToRot = {
        {RIGHT, 0.0f},
        {UP, 270.0f},
        {LEFT, 180.0f},
        {DOWN, 90.0f}
    };

    //bool collision = CheckPlayerWallCollision();
    HandleDotCollisions();
    HandlePowerDotCollisions();
    HandleGhostCollisions();

    bool queuedCollision = CheckPlayerWallCollision(Player->QueuedDirection);
    bool currentCollision = CheckPlayerWallCollision(Player->CurrentDirection);
    bool snapBothAxes;
    if (!queuedCollision) {
        
        // if ((Player->CurrentDirection != Player->QueuedDirection) &&
        //     (dirToVec(Player->CurrentDirection) != -dirToVec(Player->QueuedDirection)))
        // {   
        if (Player->CurrentDirection == Player->QueuedDirection) {
            
            SnapPlayerToGrid(Player->QueuedDirection, snapBothAxes = false);
        }
        //}
        Player->CurrentDirection = Player->QueuedDirection;
        //Player->QueuedDirection = NONE;
        Player->Rotation = dirToRot[Player->CurrentDirection];
        Player->Facing = Player->CurrentDirection;

    }


    if (currentCollision) {
        SnapPlayerToGrid(Player->CurrentDirection, snapBothAxes = true);
        Player->CurrentDirection = NONE;
        
        
        
        // SNAP PLAYER CENTER to grid, therefore updating current sensor
        // calc diff in player center and tile center, then offset player pos by that amount

        // use floor to determine which row we are on


    }



}

void Game::ActivatePowerMode() {
    std::cout << "Power mode activated" << std::endl;

    // Go through every ghost and change their color
    for (Ghost* ghost : Ghosts) {
        ghost->EnterFrightened();

    }
    //std::vector<Ghost*> Ghosts;
    
}

void Game::HandleGhostCollisions() {
    bool collision;
    for (Ghost* ghost : Ghosts) {


        collision =  (Player->Col == ghost->Col && Player->Row == ghost->Row);
        if (!collision) {
            continue;
        }
        if (ghost->CurrentMode == FRIGHTENED) {
            ghost->EnterEaten();
            Player->EnterEatingState();
            FreezeAllGhosts(1.0f);
        }
        // std::cout << " = = = = = = = = = =" << std::endl;
        // std::cout << " Collided with Ghost. " << std::endl;
        // std::cout << " = = = = = = = = = =" << std::endl;

        }
    
}

void Game::FreezeAllGhosts(float duration) {
    for (Ghost* ghost : Ghosts) {
        ghost->FreezeTimer = duration;
    }
}
void Game::HandlePowerDotCollisions() {
bool collision;
    glm::vec2 playerCenterBoxPos(Player->Position + (Player->Size / 4.0f));
    glm::vec2 playerHalfSize = (Player->Size / 2.0f);
    for (int i = 0; i < powerDots.size(); i++) {
        collision = CheckCollision(playerCenterBoxPos, playerHalfSize, powerDots[i]->Position+TILE_SIZE/2.0f, powerDots[i]->Size/2.0f);
        if (collision && !powerDots[i]->Destroyed) {
            powerDots[i]->Destroyed = true;

            ActivatePowerMode();
            remainingDots -= 1;
            Player->FreezeFrameTimer = 3.0f/60.0f;
            if (remainingDots <= 0) {
                Win();
            }
        }
    }
}
void Game::HandleDotCollisions() {
    bool collision;
    glm::vec2 playerCenterBoxPos(Player->Position + (Player->Size / 4.0f));
    glm::vec2 playerHalfSize = (Player->Size / 2.0f);
    for (int i = 0; i < dots.size(); i++) {
        collision = CheckCollision(playerCenterBoxPos, playerHalfSize, dots[i]->Position, dots[i]->Size);
        if (collision && !dots[i]->Destroyed) {
            dots[i]->Destroyed = true;
            remainingDots -= 1;
            Player->FreezeFrameTimer = 1.0f/60.0f;
            //std::cout << " Collided with dot " << std::endl;
            if (remainingDots <= 0) {
                Win();
            }
        }
    }
}

void Game::Win() {
    std::cout << "===================" << std::endl;
    std::cout << "P L A Y E R   W I N" << std::endl;
    std::cout << "===================" << std::endl;
}

void Game::SnapPlayerToGrid(Direction direction, bool bothAxes) {

    
    glm::vec2 prevPlayerPosition = Player->Position;
    // snap player to correct position for tile
    glm::vec2 currTilePos = glm::vec2(Player->Col * TILE_SIZE, Player->Row * TILE_SIZE);
    glm::vec2 currTileCenter = glm::vec2(currTilePos.x+TILE_SIZE/2.0f, currTilePos.y+TILE_SIZE/2.0f);

    if (direction == UP || direction == DOWN || bothAxes) {
        Player->Position = glm::vec2(currTileCenter.x-Player->Size.x/2.0f, Player->Position.y);// + dirToVec(Player->CurrentDirection);

    }
    else if (direction == LEFT || direction == RIGHT || bothAxes) {
        Player->Position = glm::vec2(Player->Position.x, currTileCenter.y-Player->Size.y/2.0f);// + dirToVec(Player->CurrentDirection);

    }
    //Player->Position = glm::vec2(currTileCenter.x-Player->Size.x/2.0f, currTileCenter.y-Player->Size.y/2.0f);// + dirToVec(Player->CurrentDirection);


    if (prevPlayerPosition != Player->Position) {
        // std::cout << "Snap Direction: " << direction << std::endl;
        // std::cout << "Snapped from: " << prevPlayerPosition.x << " " << prevPlayerPosition.y << std::endl;
        // std::cout << "Snapped to: " << Player->Position.x << " " << Player->Position.y << std::endl;

    }
}
// function will return true or false if there is a collision in that sensor direction
bool Game::CheckPlayerWallCollision(Direction direction) {

    glm::vec2 sensorPos;
    glm::vec2 sensorSize;
    float sensorWidth = 1.0f;
    glm::vec2 playerCenter(Player->Position.x + Player->Size.x/2.0f, Player->Position.y + Player->Size.y/2.0f);
    float halfTile = TILE_SIZE / 2.0f;
    float sensorPad = 1.0f;
    if (direction == RIGHT) { 
        sensorPos = glm::vec2(playerCenter.x + halfTile, playerCenter.y - halfTile + sensorPad);
        sensorSize = glm::vec2(sensorWidth, TILE_SIZE - sensorPad*2);
    }
    else if (direction == UP) {
        sensorPos = glm::vec2(playerCenter.x - halfTile + sensorPad, playerCenter.y - halfTile);
        sensorSize = glm::vec2(TILE_SIZE-sensorPad*2, sensorWidth);
    }
    else if (direction == LEFT) {
        sensorPos = glm::vec2(playerCenter.x - halfTile, playerCenter.y - halfTile + sensorPad);
        sensorSize = glm::vec2(sensorWidth, TILE_SIZE-sensorPad*2);
    }
    else if (direction == DOWN) {
        sensorPos = glm::vec2(playerCenter.x - halfTile + sensorPad, playerCenter.y + halfTile);
        sensorSize = glm::vec2(TILE_SIZE-sensorPad*2, sensorWidth);
    }

    if (direction == Player->CurrentDirection) {
        CurrentSensor->Position = sensorPos;
        CurrentSensor->Size = sensorSize;
    }
    if (direction == Player->QueuedDirection) {
        QueuedSensor->Position = sensorPos;
        QueuedSensor->Size = sensorSize;
    }

    // loop through all walls
    for (int i = 0; i < walls.size(); i ++) {
        if (CheckCollision(sensorPos, sensorSize, walls[i]->Position, walls[i]->Size)) {
            return true;
        }
    }
    return false;
}

bool Game::CheckCollision(glm::vec2 onePos, glm::vec2 oneSize, glm::vec2 twoPos, glm::vec2 twoSize) {
    return (((
        (onePos.x + oneSize.x >= twoPos.x) &&
        (twoPos.x + twoSize.x >= onePos.x) &&
        (onePos.y + oneSize.y >= twoPos.y) &&
        (twoPos.y + twoSize.y >= onePos.y))));
}


void Game::ProcessInput(float dt) {
    if (this->State != GAME_ACTIVE) {
        return;
    }

    bool right = this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT];
    bool up = this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP];
    bool left = this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT];
    bool down = this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN];

    float velocity = PLAYER_VELOCITY * dt;

    if (right) {
        Player->QueuedDirection = RIGHT;
        //Player->Rotation = 0.0f;
    }
    if (up) {
        Player->QueuedDirection = UP;
        //Player->Rotation = 270.0f;
    }
    if (left) {
        Player->QueuedDirection = LEFT;
        //Player->Rotation = 180.0f;
    }
    if (down) {
        Player->QueuedDirection = DOWN;
        //Player->Rotation = 90.0f;
    }

}

void Game::Render() {

    if (this->State != GAME_ACTIVE) {
        return;
    }
    Background->Draw(*Renderer);
    //MapTiles->Draw(*Renderer);

    //w.Draw(*Renderer);
    // std::cout << walls.size() << std::endl;
    for (int i = 0; i < walls.size(); i++) {
        //walls[i]->Draw(*Renderer);
    }
    for (int i = 0; i < dots.size(); i++) {
        if (!dots[i]->Destroyed)
            dots[i]->Draw(*Renderer);
    }
    for (int i = 0; i < powerDots.size(); i++) {
        if (!powerDots[i]->Destroyed)
            powerDots[i]->Draw(*Renderer);
    }
    //Wall->Draw(*Renderer);
    //Wall2->Draw(*Renderer);
    
    Player->Draw(*Renderer);
    for (Ghost* ghost : Ghosts) {
        ghost->Draw(*Renderer);
        //ghost->DebugMapTarget->Draw(*Renderer);
        ghost->EyesObj->Draw(*Renderer, ghost->EyesObj->currSprite);
    }

    // NewGhost->Draw(*Renderer);
    // NewGhost->DebugMapTarget->Draw(*Renderer);
    // NewGhost->EyesObj->Draw(*Renderer, NewGhost->EyesObj->currSprite);
    // RedGhost->Draw(*Renderer);
    // RedGhostEyes->Draw(*Renderer, RedGhostEyes->currSprite);
    // RedGhostMapTarget->Draw(*Renderer);
    // RedGhostAdjTarget->Draw(*Renderer);
    // PinkGhost->Draw(*Renderer);
    // PinkGhostEyes->Draw(*Renderer, PinkGhostEyes->currSprite);
    // PinkGhostMapTarget->Draw(*Renderer);
    // PinkGhostAdjTarget->Draw(*Renderer);


    //QueuedSensor->Draw(*Renderer);
    //CurrentSensor->Draw(*Renderer);
   
}

void PrintFPS(float dt) {
    // display fps
    float frames = (1.0/dt);
    fpsQueue.push(frames);
    queueSum += frames;

    if (fpsQueue.size() > 600) {
        float f = fpsQueue.front();
        fpsQueue.pop();
        queueSum -= f;
    }
    std::cout << queueSum / fpsQueue.size() << std::endl;
}
