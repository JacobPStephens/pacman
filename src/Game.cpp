#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PlayerObject.h"

#include <iostream>
#include <tuple>
#include <queue>
#include <vector>

SpriteRenderer* Renderer;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) 
{

}

Game::~Game() {
    delete Renderer;

}

std::queue<float> fpsQueue;
float queueSum;

const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);
const float PLAYER_VELOCITY(500.0f);
PlayerObject *Player;

// const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// const float BALL_RADIUS = 12.5f;
// BallObject *Ball;

void Game::Init() {


    ResourceManager::LoadTexture("textures/pacman.png", true, "pacman");
    ResourceManager::LoadTexture("textures/pacman2.png", true, "pacman2");
    glm::vec2 playerPos = glm::vec2(
        this->Width/2.0f - PLAYER_SIZE.x/2.0f,
        this->Height/2.0f - PLAYER_SIZE.y/2.0f
    );

    std::vector<Texture2D> sprites = {
        ResourceManager::GetTexture("pacman"),
        ResourceManager::GetTexture("pacman2")
    };

    Player = new PlayerObject(playerPos, PLAYER_SIZE, glm::vec2(1.0f), sprites, 0.25f);

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

void Game::Update(float dt) {
    Player->Update(dt);
    //Player->Move(dt);
    //PrintFPS(dt);




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
        Player->Direction = glm::vec2(1.0f, 0.0f);
        Player->Rotation = 0.0f;
    }
    if (up) {
        Player->Direction = glm::vec2(0.0f, -1.0f);
        Player->Rotation = 270.0f;
    }
    if (left) {
        Player->Direction = glm::vec2(-1.0f, 0.0f);
        Player->Rotation = 180.0f;
    }
    if (down) {
        Player->Direction = glm::vec2(0.0f, 1.0f);
        Player->Rotation = 90.0f;
    }

}

void Game::Render() {

    if (this->State != GAME_ACTIVE) {
        return;
    }

    Player->Draw(*Renderer);

    // seg fault Renderer->DrawSprite(ResourceManager::GetTexture("background"),
    // glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
    // );

   
}
