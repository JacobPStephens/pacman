#include "PlayerObject.h"
#include <iostream>
// PlayerObject::PlayerObject()
//     : GameObject() {};

PlayerObject::PlayerObject(glm::vec2 position, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, float cycleLength, std::string name)
    : GameObject(position, size, sprites, cycleLength, glm::vec3(1.0f), velocity, name) {
        this->CurrentDirection = RIGHT;
    };



void PlayerObject::Update(float dt, float screenWidth) {
    GameObject::Update(dt);
    this->Move(dt, screenWidth);

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

