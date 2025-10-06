#include "Ghost.h"
#include <iostream>

// TODO: Just got the map implemented as a field of the game class. Use that in this GetAdjDirection function.
// Maybe clean up constuctor to only have what's necesary. I feel like I don't have to pass the map every update.
Ghost::Ghost(glm::vec2 position, glm::vec2 size, glm::vec2 velocity, std::vector<Texture2D> sprites, float rotation, std::string name, float tileSize, PlayerObject* player, std::vector<std::vector<int>>* map, GameObject* ghostMapTarget, GameObject* ghostAdjTarget)
    : GameObject(position, size, tileSize, sprites, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, glm::vec2(0.0f), std::string("ghost"))
{ 
    // game state variables
    this->TILE_SIZE = tileSize; 
    this->Map = map;

    // set attributes of ghost
    this->Speed = ((39.75f * this->TILE_SIZE) / 8.0f);
    this->Row = 22;
    this->Col = 12;

    // init movement logic variables
    glm::vec2 tileCenter = glm::vec2(this->Col, this->Row) * tileSize + tileSize/2.0f;
    this->Position = tileCenter - this->Size / 2.0f;
    this->AdjTargetPos = glm::vec2(this->Col, this->Row);

    // init debug target objects
    this->ghostMapTarget = ghostMapTarget;
    this->ghostMapTarget->Color = glm::vec3(1.0f, 0.0f, 0.0f);
    this->ghostMapTarget->Size = glm::vec2(2.0f);

    this->ghostAdjTarget = ghostAdjTarget;
    this->ghostMapTarget->Color = glm::vec3(1.0f, 0.0f, 0.0f);
    this->ghostAdjTarget->Size = glm::vec2(2.0f);



}

// every time the ghost reaches a tile, we have to calculate a new direction.

//// OPTION A
// 1. detect when a ghost reaches a tile
// 2. calculate new direction
// 3. move in that direction until ghost reaches a new tile (fixed duration?)

//// OPTION B
// 1. calculate new direction
// 2. move in that direction for fixed length




// speed = distance / time
// time = distance / speed
// time to move one tile = length of tile / speed of ghost
// move for (length of tile) / (speed of ghost) seconds at (speed of ghost) speed

void Ghost::Update(float deltaTime, PlayerObject* player) {
    GameObject::Update(deltaTime);

    // set map target (ghost color dependent)
    this->MapTargetPos = glm::vec2(player->Col, player->Row);
    
    // calculate row and column of ghost
    this->Row = floor(this->CenterPos.y/ (int)this->TILE_SIZE);
    this->Col = floor(this->CenterPos.x / (int)this->TILE_SIZE);

    // draw debug sprites
    ghostMapTarget->Position = this->MapTargetPos * this->TILE_SIZE;
    ghostMapTarget->Size = glm::vec2(this->TILE_SIZE);
    ghostAdjTarget->Position = (this->AdjTargetPos * this->TILE_SIZE) + (this->TILE_SIZE/2.0f);
    


    // if adj target reached, get new adj target and direction towards it
    if (AdjTargetReached(glm::vec2(this->CenterPos.x, this->CenterPos.y), this->AdjTargetPos)) {
        this->AdjTargetPos = GetAdjTarget(this->Position, this->Size, this->Dir, this->MapTargetPos, this->TILE_SIZE, this->Map);//this->Position, this->Size, this->Dir, this->MapTargetPos, tileSize, map);
        this->Dir = GetDirection(this->Position, this->AdjTargetPos);
    }

    // move ghost in correct direction
    this->Position = Move(deltaTime, this->Position, this->Dir, this->Speed);   

}


/* Returns new position of ghost using current position, speed, and direction*/
glm::vec2 Ghost::Move(float deltaTime, glm::vec2 pos, Direction dir, float speed) {
    return pos + dirToVec(dir) * speed * deltaTime;
}

/* Return if ghost has reached adjacent tile, given its center and target tile */
bool Ghost::AdjTargetReached(glm::vec2 ghostCenter, glm::vec2 adjTargetPos) {
    // get target coordinates in pixels from target tile
    glm::vec2 pixelTarget = (adjTargetPos * this->TILE_SIZE) + (this->TILE_SIZE/2.0f);

    // compare ghost center to pixel coordinate
    float threshold = 0.01f;
    bool xReached = abs(pixelTarget.x - ghostCenter.x) <= threshold;
    bool yReached = abs(pixelTarget.y - ghostCenter.y) <= threshold;

    // reached in both axes
    return xReached && yReached;
}

/*
This function updates ghost direction and
returns adjacent target tile position
*/

Direction Ghost::GetDirection(glm::vec2 ghostPos, glm::vec2 adjTargetPos) {
    float threshold = 0.001f;
    float deltaX = adjTargetPos.x - this->Col;
    float deltaY = adjTargetPos.y - this->Row;
    
    if (deltaX >= 0.0f && abs(deltaY) <= threshold) {
        return RIGHT;
    }
    else if (deltaX < 0.0f && abs(deltaY) <= threshold) {
        return LEFT;
    }
    else if (deltaY >= 0.0f && abs(deltaX) <= threshold) {
        return DOWN;
    }
    else if (deltaY < 0.0f && abs(deltaX) <= threshold) {
        return UP;
    }
    else {
        std::cout << "ERROR::Invalid Direction!!" << std::endl;
        return NONE;
    }
}
// Remember adj target is in terms of tiles
glm::vec2 Ghost::GetAdjTarget(glm::vec2 ghostPos, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, float tileSize, std::vector<std::vector<int>>* map) {

    // float ghostX = (ghostPos.x / tileSize); // center
    // float ghostY = (ghostPos.y / tileSize);
    // std::tuple<int, int> ghostTilePos = std::make_tuple(ghostX, ghostY);

    // get ghost center, then calculate its tiles
    glm::vec2 ghostCenter = glm::vec2(ghostPos.x + ghostSize.x / 2.0f, ghostPos.y + ghostSize.y / 2.0f);
    this->Row = floor(ghostCenter.y / (int)tileSize);
    this->Col = floor(ghostCenter.x / (int)tileSize);


    Direction dirs[4] = {LEFT, UP, RIGHT, DOWN};
    Direction closestNodeToMapTarget;
    float minDist = tileSize * 9999.0f;
    for (Direction dir : dirs) {
        glm::vec2 dirVec = dirToVec(dir);

        // filter out 180 deg turn or wall tiles
        if (dirVec == -dirToVec(currDir)) {
            //std::cout << "Filtered out direction (backtracking) " << dir << std::endl;
            continue;
        }
        int tile = map->at(this->Row+dirVec.y).at(this->Col+dirVec.x);
        if (tile == 0) {
            //std::cout << "Filtered out direction (wall)" << dir << std::endl;
            continue;
        }


        float dist = GetDistance(this->Col + dirVec.x, this->Row + dirVec.y, mapTargetPos.x, mapTargetPos.y);

        if (dist < minDist) {
            closestNodeToMapTarget = dir;
            minDist = dist;
        }

    }
    // //std::cout << "Calculated direction inside get adj target=" << closestNodeToMapTarget << std::endl;
    // float adjTargetX = ghostPos.x + dirToVec(closestNodeToMapTarget).x * tileSize;
    // float adjTargetY = ghostPos.y + dirToVec(closestNodeToMapTarget).y * tileSize;
    //std::cout << "Min distance is in direction " << closestNodeToMapTarget << std::endl;
    float adjTargetX = this->Col + dirToVec(closestNodeToMapTarget).x;
    float adjTargetY = this->Row + dirToVec(closestNodeToMapTarget).y;

    return glm::vec2(adjTargetX, adjTargetY);

    // std::cout << "ghost tile pos: " << std::get<0>(ghostTilePos) << " " << std::get<1>(ghostTilePos) << std::endl;
    // // for (int r = 0; r < map.size(); r++) {
    // //     for (int c = 0; c < map[r].size(); c++) {


    // //     }
    // // }



    //return glm::vec2(0.0f);

    // return glm::vec2((float)x, (float)y);
}

float Ghost::GetDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2-x1, 2.0f) + pow(y2-y1,2.0f));
}


Ghost::~Ghost() {
}
