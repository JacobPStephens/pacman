#include "PlayerObject.h"
#include <iostream>
// PlayerObject::PlayerObject()
//     : GameObject() {};

PlayerObject::PlayerObject(glm::vec2 position, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, float cycleLength)
    : GameObject(position, size, sprites, cycleLength, glm::vec3(1.0f), velocity) {

        this->Speed = 250.0f;
        this->Direction = glm::vec2(1.0f, 0.0f);

    };



void PlayerObject::Update(float dt) {
    GameObject::Update(dt);
    this->Move(dt);

}

glm::vec2 PlayerObject::Move(float dt) {

    //std::cout << Sprites << std::endl;

    this->Position += this->Direction * dt * this->Speed;
    return glm::vec2(0.0f);

}

