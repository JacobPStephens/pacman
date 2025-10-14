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



    if (this->EatTimer > 0.0f) {
        this->EatTimer -= dt;
    }
    else if (this->CurrentMode == EATING) {
        ExitEatingState();
    }
    this->Move(dt, screenWidth);
    glm::vec2 playerCenter = glm::vec2(this->Position.x + this->Size.x / 2.0f, this->Position.y + this->Size.y/2.0f);
    this->Row = floor(playerCenter.y / this->TILE_SIZE);
    this->Col = floor(playerCenter.x / this->TILE_SIZE);
    //std::cout << "player is on row, col" << this->Row << "," << this->Col << std::endl; // off by one thing
    //std::cout << this->CurrentDirection << " " << dirToVec(this->CurrentDirection).x << " " << dirToVec(this->CurrentDirection).y << std::endl;
}

void PlayerObject::EnterEatingState() {
    this->CurrentMode = EATING;
    this->EatTimer = 1.0f;
    this->Alpha = 0.25f;
    
}

void PlayerObject::ExitEatingState() {
    this->CurrentMode = FEASTING;
    this->Alpha = 1.0f;


}
glm::vec2 PlayerObject::Move(float dt, float screenWidth) {

    if (this->FreezeFrameTimer > 0) {
        this->FreezeFrameTimer -= dt;
        //std::cout << "Skipped one mvmt frame due to freeze frames: " << this->FreezeFrameTimer << std::endl;

        return glm::vec2(this->Position.x, this->Position.y);
    }

    if (this->CurrentMode == EATING) {
        return glm::vec2(this->Position.x, this->Position.y);
    }

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

    return glm::vec2(this->Position.x, this->Position.y);

}

