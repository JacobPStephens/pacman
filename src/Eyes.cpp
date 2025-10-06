#include "Eyes.h"
#include <iostream>
// ghost eyes dimensions are 5 width by 10 length

Eyes::Eyes(Ghost* parentGhost, float tileSize, std::vector<Texture2D> sprites) 
    :GameObject(glm::vec2(1.0f), glm::vec2(1.0f), tileSize, sprites, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, glm::vec2(0.0f), std::string("eyes"))
{   
    this->parentGhost = parentGhost;
    this->CycleLength = 5.0f;
    this->currSprite = 0;
    this->Size = glm::vec2(14.0f/8.0f * tileSize);
}

void Eyes::Update(float deltaTime) {
    GameObject::Update(deltaTime);
    this->Position = this->parentGhost->Position;
    SetSprite(this->parentGhost->Dir);

    //std::cout << this->currSprite << std::endl;
    //std::cout << this->parentGhost->Dir << std::endl;
}

void Eyes::SetSprite(Direction direction) {
    switch (direction) {
        case RIGHT:
            this->currSprite = 0;
            break;
        case UP:
            this->currSprite = 1;
            break;
        case LEFT:
            this->currSprite = 2;
            break;
        case DOWN:
            this->currSprite = 3;
            break;
        default:
            std::cout << "ERROR::UNEXPECTED DIRECTION IN EYE SET SPRITE" << std::endl;
            this->currSprite = 0;
    }
}

