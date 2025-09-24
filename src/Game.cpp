#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Wall.h"

#include <iostream>
#include <fstream>
#include <queue>

#include <filesystem>

SpriteRenderer* Renderer;
std::queue<float> fpsQueue;
float queueSum;

const float RESOLUTION_SCALE = 4.0f;
const float TILE_SIZE = 8.0f * RESOLUTION_SCALE;
const glm::vec2 PLAYER_SIZE((13.0f * TILE_SIZE) / 8.0f, (13.0f * TILE_SIZE) / 8.0f);
const float PLAYER_VELOCITY((35.0f * TILE_SIZE) / 8.0f);
PlayerObject* Player;
//GameObject* Wall;
//GameObject* Wall2;
GameObject* Background;
GameObject* CurrentSensor;
GameObject* QueuedSensor;
std::vector<Wall*> walls;



glm::vec2 dirToVec(Direction direction) {
    switch (direction) {
        case RIGHT:
            return glm::vec2(1.0f, 0.0f);
        case UP:
            return glm::vec2(0.0f, -1.0f);
        case LEFT:
            return glm::vec2(-1.0f, 0.0f);
        case DOWN:
            return glm::vec2(0.0f, 1.0f);
        default:
            return glm::vec2(0.0f);
    }
}

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{

}

Game::~Game() {
    delete Renderer;
    delete Player;
    //delete Wall;
    //delete Wall2;
    delete CurrentSensor;
    delete QueuedSensor;
    delete Background;

    // delete all wall ptrs
    for (int i = 0; i < walls.size(); i++) {
        delete walls[i];
    }
}


// const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// const float BALL_RADIUS = 12.5f;
// BallObject *Ball;

void Game::Init() {


    ResourceManager::LoadTexture("textures/pacmanNew1.png", true, "pacman");
    ResourceManager::LoadTexture("textures/pacmanNew2.png", true, "pacman2");
    ResourceManager::LoadTexture("textures/map.png", false, "walls");
    ResourceManager::LoadTexture("textures/grayWall.png", true, "grayWall");
    ResourceManager::LoadTexture("textures/px.png", true, "px");
    ResourceManager::LoadTexture("textures/map224x248.png", true, "map224x248");
    
    std::vector<Texture2D> wallSprites = { ResourceManager::GetTexture("map224x248")};

    glm::vec2 mapSize = glm::vec2(224.0f * RESOLUTION_SCALE, 248.0f * RESOLUTION_SCALE);
    glm::vec2 mapPos = glm::vec2(this->Width/2.0f - mapSize.x/2.0f, 0.0f);

    Background = new GameObject(mapPos, mapSize, wallSprites, 1.0f, glm::vec3(1.0f), glm::vec3(0.0f), std::string("wall"));

    std::vector<Texture2D> grayWallSprites = { ResourceManager::GetTexture("px")};


    std::string line;
    std::ifstream wallInput;

    wallInput.open("/home/jacob/source/pacman/util/walls.txt");

    if (!wallInput.is_open()) {
        std::cout <<"WALL FILE DID NOT OPEN" << std::endl;
    }

    //std::array<std::array<std::string, 4>, 32> wallTiles;
    std::vector<std::vector<int>> wallTiles;
    std::vector<int> wall;

    while (std::getline (wallInput, line)) {

        //wall.push_back(std::stoi(num));
        std::cout << "Line: " << line << std::endl;

        std::string num;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != ' ' && line[i] != '\n') {
                num += line[i];
            }
            else {
                //std::cout << num << std::endl;
                wall.push_back(std::stoi(num));
                num = "";
            }
        }
        //std::cout << num << std::endl;
        wall.push_back(std::stoi(num));
        //num = "";


        wallTiles.push_back(wall);
        wall.clear();

        if (wall.size() == 4) {
            wallTiles.push_back(wall);
            std::cout << "Added wall: " << wall[0] << wall[1] << wall[2]<< wall[3] << std::endl;
            wall.clear();

        }
    }

    wallInput.close();


    // for (int i = 0; i < wallTiles.size(); i++) {
    //     for (int j = 0; j < wallTiles[i].size(); j++) {
    //         std::cout << wallTiles[i][j] * TILE_SIZE << ' ';
        
    //     }
    //     std::cout << '\n';
    // }

    //w = Wall(glm::vec2(0.0f), glm::vec2(50.0f), grayWallSprites, 0);

    //std::vector<Wall*> walls;
    for (int i = 0; i < wallTiles.size(); i++) {
        //std::cout << wallTiles[i][j] * TILE_SIZE << ' ';

        glm::vec2 pos(wallTiles[i][0] * TILE_SIZE, wallTiles[i][1] * TILE_SIZE);
        glm::vec2 size(wallTiles[i][2] * TILE_SIZE, wallTiles[i][3] * TILE_SIZE);
        Wall* w = new Wall(pos, size, grayWallSprites, i);
        std::cout << "Wall created" << std::endl;
        walls.push_back(w);
    
    }

       // std::cout << text << std::endl;
        // std::array<std::string, 4> wall;
        // std::string part;

        // int wallIdx = 0;
        // for (int i = 0; i < text.size(); i++) {
        //     std::cout << text[i] << std::endl;
        //     if (text[i] == ' ') {
                
        //     }
        //     num += text[i];
        // }

    //std::tuple<5, 5>
    // store all wall positions (don't store symmetrical walls)1
    
    // 20 horizontal tile lines
    std::array<float, 20> hLines = {
        38.0f, 99.0f, 151.0f
        //0.0f, 0.0f
    };

    // 17 vertical tile lines
    std::array<float, 17> vLines = {
        70.5f, 133.0f, 215.0f
    };

    for (int i = 3; i < 20; i++) {
        float diff = (i % 2 == 1) ? 60.0f : 24.5f;

        hLines[i] = hLines[i-1] + diff;
        if (i < 15) {
            vLines[i] = vLines[i-1] + diff;
        }
    }
    // add last two vert lines that don't match 
    vLines[15] = 780.0f;
    vLines[16] = 838.0f;


    // for (int i = 0; i < hLines.size(); i++) {
    //     wallInfo[i] = std::make_tuple(glm::vec2(70.5f, hLines[i]), glm::vec2(mapSize.x/2.0f, wallWidth));
    // }
    // for (int i = 0; i < vLines.size(); i++) {
    //     wallInfo[i+hLines.size()] = std::make_tuple(glm::vec2(vLines[i], 38.0f), glm::vec2(wallWidth, mapSize.y/2.0f));
    // } 

    // make walls


    // // top wall
    // std::array<std::tuple<glm::vec2, glm::vec2>, 25> wallInfo = {
    //     AssembleWallInfo(vLines[0], hLines[0], vLines[16], hLines[0]),
    //     AssembleWallInfo(vLines[1], hLines[1], vLines[2], hLines[1]),
    //     AssembleWallInfo(vLines[3], hLines[1], vLines[6], hLines[1]),
    //     AssembleWallInfo(vLines[1], hLines[2], vLines[2], hLines[2]),
    //     AssembleWallInfo(vLines[3], hLines[2], vLines[6], hLines[2]),
    //     AssembleWallInfo(vLines[1], hLines[3], vLines[2], hLines[3]),
    //     AssembleWallInfo(vLines[1], hLines[4], vLines[2], hLines[4]),
    //     AssembleWallInfo(vLines[0], hLines[5], vLines[2], hLines[5]), 
    //     AssembleWallInfo(vLines[4], hLines[5], vLines[6], hLines[5]),
    //     AssembleWallInfo(vLines[4], hLines[6], vLines[6], hLines[6]),
        
    //     // bottom half
    //     AssembleWallInfo(vLines[0], hLines[12], vLines[2], hLines[12]), 
    //     AssembleWallInfo(vLines[1], hLines[13], vLines[2], hLines[13]), 
    //     AssembleWallInfo(vLines[1], hLines[14], vLines[2], hLines[14]), 
    //     AssembleWallInfo(vLines[0], hLines[15], vLines[1], hLines[15]), 
    //     AssembleWallInfo(vLines[0], hLines[16], vLines[1], hLines[16]), 
    //     AssembleWallInfo(vLines[1], hLines[17], vLines[3], hLines[17]), 
    //     AssembleWallInfo(vLines[1], hLines[18], vLines[6], hLines[18]), 
    //     AssembleWallInfo(vLines[3], hLines[3], vLines[4], hLines[3]), 
    //     AssembleWallInfo(vLines[3], hLines[12], vLines[4], hLines[12]), 
    //     AssembleWallInfo(vLines[3], hLines[15], vLines[4], hLines[15]), 
    //     AssembleWallInfo(vLines[3], hLines[13], vLines[6], hLines[13]),
    //     AssembleWallInfo(vLines[3], hLines[14], vLines[6], hLines[14]),
    //     std::make_tuple(glm::vec2(vLines[0], hLines[0]), glm::vec2(vLines[16]-vLines[0], wallWidth)),
    // std::make_tuple(glm::vec2(vLines[0], hLines[0]), glm::vec2(vLines[16]-vLines[0], wallWidth)),
    //};


    // for (int i = 0; i < wallInfo.size(); i++) {
    //     glm::vec2 pos = std::get<0>(wallInfo[i]);

    //     glm::vec2 size = std::get<1>(wallInfo[i]);

    //     Wall* tmpWall = new Wall(pos, size, grayWallSprites, i);
    //     walls.push_back(tmpWall);
    // };





 


    // Wall = new GameObject(glm::vec2(70.5f, 0.0f), glm::vec2(2.0f, 200.0f), grayWallSprites, 1.0f, glm::vec3(1.0f), glm::vec3(0.0f), std::string("grayWall"));
    // Wall2 = new GameObject(glm::vec2(this->Width-70.5f, 0.0f), glm::vec2(2.0f, 200.0f), grayWallSprites, 1.0f, glm::vec3(1.0f), glm::vec3(0.0f), std::string("grayWall"));
    // walls.push_back(Wall);
    // walls.push_back(Wall2);
    CurrentSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), grayWallSprites, 1.0f, glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f), "upSensor");
    QueuedSensor = new GameObject(glm::vec2(0.0f), glm::vec2(1.0f), grayWallSprites, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f), "upSensor");
    

    glm::vec2 playerPos = glm::vec2(
        this->Width/2.0f - PLAYER_SIZE.x/2.0f + 40.0f,
        this->Height/2.0f - PLAYER_SIZE.y/2.0f
    );

    std::vector<Texture2D> sprites = {
        ResourceManager::GetTexture("pacman"),
        ResourceManager::GetTexture("pacman2")
    };
    Player = new PlayerObject(playerPos, PLAYER_SIZE, glm::vec2(PLAYER_VELOCITY), sprites, 0.25f, std::string("player"));

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
    Player->Update(dt, 224.0f * RESOLUTION_SCALE);
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

    bool queuedCollision = CheckPlayerWallCollision(Player->QueuedDirection);
    bool currentCollision = CheckPlayerWallCollision(Player->CurrentDirection);

    if (!queuedCollision) {
        Player->CurrentDirection = Player->QueuedDirection;
        //Player->QueuedDirection = NONE;
        Player->Rotation = dirToRot[Player->CurrentDirection];
    }


    if (currentCollision) {
        Player->CurrentDirection = NONE;
    }



}

// function will return true or false if there is a collision in that sensor direction
bool Game::CheckPlayerWallCollision(Direction direction) {

    glm::vec2 sensorPos;
    glm::vec2 sensorSize;
    float sensorPad = 1.0f;
    if (direction == RIGHT) { 
        sensorPos = glm::vec2(Player->Position.x + Player->Size.x + sensorPad, Player->Position.y);
        sensorSize = glm::vec2(1.0f, Player->Size.y);
        sensorSize = glm::vec2(1.0f, 1.0f);
    }
    else if (direction == UP) {
        sensorPos = glm::vec2(Player->Position.x, Player->Position.y - sensorPad);
        sensorSize = glm::vec2(Player->Size.x, 1.0f);
        sensorSize = glm::vec2(1.0f, 1.0f);
    }
    else if (direction == LEFT) {
        sensorPos = glm::vec2(Player->Position.x - sensorPad, Player->Position.y);
        sensorSize = glm::vec2(1.0f, Player->Size.y);
        sensorSize = glm::vec2(1.0f, 1.0f);
    }
    else if (direction == DOWN) {
        sensorPos = glm::vec2(Player->Position.x, Player->Position.y + Player->Size.y + sensorPad);
        sensorSize = glm::vec2(Player->Size.x, 1.0f);
        sensorSize = glm::vec2(1.0f, 1.0f);
    }

    if (direction == Player->CurrentDirection) {
        CurrentSensor->Position = sensorPos;
        CurrentSensor->Size = sensorSize;
        //std::cout << "Moving current sensor" << std::endl;
    }
    if (direction == Player->QueuedDirection) {
        QueuedSensor->Position = sensorPos;
        QueuedSensor->Size = sensorSize;
        //std::cout << "Moving queued sensor" << std::endl;
    }

    // loop through all walls
    for (int i = 0; i < walls.size(); i ++) {
        if (CheckCollision(sensorPos, sensorSize, walls[i]->Position, walls[i]->Size)) {
            return true;
        }
    }
    return false;
}

// bool Game::CheckPlayerWallCollision() {
    
//     float sensorPad = 1.0f;
//     // right sensor
//     glm::vec2 rightSensorPos = glm::vec2(
//         Player->Position.x + Player->Size.x + sensorPad,
//         Player->Position.y
//     );
//     // up sensor
//     glm::vec2 upSensorPos = glm::vec2(
//         Player->Position.x,
//         Player->Position.y - sensorPad
//     );
//     // left sensor
//     glm::vec2 leftSensorPos = glm::vec2(
//         Player->Position.x - sensorPad,
//         Player->Position.y
//     );

//     glm::vec2 downSensorPos = glm::vec2(
//         Player->Position.x,
//         Player->Position.y + (Player->Size.y) + sensorPad
//     );
//     RightSensor->Position = rightSensorPos;
//     RightSensor->Size = glm::vec2(3.0f, Player->Size.y);
    
//     UpSensor->Position = upSensorPos;
//     UpSensor->Size = glm::vec2(Player->Size.x, 3.0f);

//     LeftSensor->Position = leftSensorPos;
//     LeftSensor->Size = glm::vec2(3.0f, Player->Size.y);

//     DownSensor->Position = downSensorPos;
//     DownSensor->Size = glm::vec2(Player->Size.x, 3.0f);

    
//     // check collision of queued direction



//     // and check collision of current direction

//     //std::cout << "Player: " << Player->Position.x << " " << Player->Position.y << " Sensor: " << rightSensorPos.x << " " << rightSensorPos.y << std::endl;
//     bool rightCollision = CheckCollision(rightSensorPos, glm::vec2(1.0f, Player->Size.y), Wall->Position, Wall->Size);
//     bool upCollision = CheckCollision(upSensorPos, glm::vec2(Player->Size.x, 1.0f), Wall->Position, Wall->Size);
//     bool leftCollision = CheckCollision(leftSensorPos, glm::vec2(1.0f, Player->Size.y), Wall->Position, Wall->Size);
//     bool downCollision = CheckCollision(downSensorPos, glm::vec2(Player->Size.x, 1.0f), Wall->Position, Wall->Size);


//     if (!rightCollision && Player->QueuedDirection == glm::vec2(1.0f, 0.0f)) {
//         Player->CurrentDirection = Player->QueuedDirection;
//         Player->QueuedDirection = glm::vec2(0.0f);
//         Player->Rotation = 0.0f;
//     }
//     else if (!leftCollision && Player->QueuedDirection == glm::vec2(-1.0f, 0.0f)) {
//         Player->CurrentDirection = Player->QueuedDirection;
//         Player->QueuedDirection = glm::vec2(0.0f);
//         Player->Rotation = 180.0f;
//     }

//     else if (!upCollision && Player->QueuedDirection == glm::vec2(0.0f, -1.0f)) {
//         Player->CurrentDirection = Player->QueuedDirection;
//         Player->QueuedDirection = glm::vec2(0.0f);
//         Player->Rotation = 270.0f;
//     }

//     else if (!downCollision && Player->QueuedDirection == glm::vec2(0.0f, 1.0f)) {
//         Player->CurrentDirection = Player->QueuedDirection;
//         Player->QueuedDirection = glm::vec2(0.0f);
//         Player->Rotation = 90.0f;
//     }

//     if (rightCollision && Player->CurrentDirection == glm::vec2(1.0f, 0.0f)) {
//         Player->CurrentDirection = glm::vec2(0.0f);
//     }
//     else if (upCollision && Player->CurrentDirection == glm::vec2(0.0f, -1.0f)) {
//         Player->CurrentDirection = glm::vec2(0.0f);
//     }
//     else if (leftCollision && Player->CurrentDirection == glm::vec2(-1.0f, 0.0f)) {
//         Player->CurrentDirection = glm::vec2(0.0f);
//     }
//     else if (downCollision && Player->CurrentDirection == glm::vec2(0.0f, 1.0f)) {
//         Player->CurrentDirection = glm::vec2(0.0f);
//     }


//     // if (rightCollision || upCollision || leftCollision|| downCollision) {
//     //     Player->CurrentDirection = glm::vec2(0.0f);
//     //     return true;
//     // }

//     return false;
// }

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

    //w.Draw(*Renderer);
    for (int i = 0; i < walls.size(); i++) {
        std::cout << "Drawing walls" << std::endl;
        std::cout << walls.size() << std::endl;
        walls[i]->Draw(*Renderer);
    }
    //Wall->Draw(*Renderer);
    //Wall2->Draw(*Renderer);
    

    Player->Draw(*Renderer);

    QueuedSensor->Draw(*Renderer);
    CurrentSensor->Draw(*Renderer);



    // seg fault Renderer->DrawSprite(ResourceManager::GetTexture("background"),
    // glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
    // );

   
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
