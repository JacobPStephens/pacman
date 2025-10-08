#include "GameObject.h"
#include <iostream>
#include <cmath>


GameObject::GameObject() 
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Alpha(1.0f), Rotation(0.0f), Destroyed(false)
{ }

GameObject::GameObject(glm::vec2 position, glm::vec2 size, const float tileSize, std::vector<Texture2D> sprites)
    : Position(position), Size(size), Sprites(sprites), CycleLength(1.0f), Color(glm::vec3(1.0f)), Alpha(1.0f), Velocity(0.0f), Lifespan(0.0f), Rotation(0.0f)
{

}
GameObject::GameObject(glm::vec2 position, glm::vec2 size, const float tileSize, std::vector<Texture2D> sprites, float cycleLength, glm::vec3 color, float alpha, float velocity, std::string name)
    : Position(position), Size(size), Sprites(sprites), CycleLength(cycleLength), Velocity(velocity), Color(color), Alpha(alpha), Rotation(0.0f), Destroyed(false), Name(name)
{ 
    this->Lifespan = 0.0f;
    this->TILE_SIZE = tileSize;
}

void GameObject::Update(float dt) {
    this->Lifespan += dt;
    this->CenterPos = this->Position + this->Size / 2.0f;

    // calculate row and column 
    this->Row = floor(this->CenterPos.y/ (int)this->TILE_SIZE);
    this->Col = floor(this->CenterPos.x / (int)this->TILE_SIZE);
    //std::cout << this->Name << std::endl;

    //std::cout << this->Name << std::endl;
    //if (this->Name == "wall") {
        //std::cout << this->Position.x << " " << this->Position.y << std::endl;
    //}
    //std::cout << this->Lifespan << std::endl;
}

void GameObject::Draw(SpriteRenderer &renderer, int stillFrame) {   

    int frame = 0;
    if (stillFrame == -1) {
        float timePerFrame = (this->CycleLength / this->Sprites.size());
        frame = glm::floor(fmod(this->Lifespan, this->CycleLength) / timePerFrame);
    }
    else {
        frame = stillFrame;
    }



    Texture2D sprite = this->Sprites[frame];

    renderer.DrawSprite(this->Sprites.at(frame), this->Position, this->Size, this->Rotation, this->Color, this->Alpha);
    
    
    // cycle time = 1.0f
    // num frames = 5
    // then each frame should get 0.2 seconds

    // so get current time modded by cycle time, and divide it by 0.2 seconds and floor it

    // so example at time = 5.2f
    // 5.2f % 1.0f = 0.2f
    // 0.2 floor division 0.2 = 1 so frame 1

    // example at time 3.76f 
    // 3.76 % 1.0 = 0.76
    // 0.76 floor division by 0.2 = 3 so frame 3

    // remember frames start at 0

    //std::cout << this->Position.x << " " << this->Position.y << std::endl;
    //std::cout << "Rendering player" << std::endl;
}