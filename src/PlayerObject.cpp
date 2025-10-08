#include "PlayerObject.h"
#include <iostream>
// PlayerObject::PlayerObject()
//     : GameObject() {};

PlayerObject::PlayerObject(glm::vec2 position, glm::vec2 size, const float tileSize, float velocity, std::vector<Texture2D> sprites, float cycleLength, std::string name)
    : GameObject(position, size, tileSize, sprites, cycleLength, glm::vec3(1.0f), 1.0f, velocity, name) {
        this->CurrentDirection = RIGHT;
    };



void PlayerObject::Update(float dt, float screenWidth) {
    GameObject::Update(dt);
    this->Move(dt, screenWidth);
    glm::vec2 playerCenter = glm::vec2(this->Position.x + this->Size.x / 2.0f, this->Position.y + this->Size.y/2.0f);
    this->Row = floor(playerCenter.y / this->TILE_SIZE);
    this->Col = floor(playerCenter.x / this->TILE_SIZE);
    //std::cout << "player is on row, col" << this->Row << "," << this->Col << std::endl; // off by one thing
    //std::cout << this->CurrentDirection << " " << dirToVec(this->CurrentDirection).x << " " << dirToVec(this->CurrentDirection).y << std::endl;
}

glm::vec2 PlayerObject::Move(float dt, float screenWidth) {

    //std::cout << Sprites << std::endl;

    this->Position += dirToVec(this->CurrentDirection) * dt * this->Velocity;
    // this->Position.x = (float)fmod((double)this->Position.x, (double)screenWidth);

    // wrapping
    if (this->Position.x < 0.0f-this->Size.x) {
        this->Position.x = screenWidth;
    }
    if (this->Position.x > screenWidth) {
        this->Position.x = 0.0f-this->Size.x;
    }

    return glm::vec2(0.0f);

}

