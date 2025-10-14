#include "Ghost.h"
#include "LevelSpecs.h"
#include <iostream>


const glm::vec2 GHOST_SIZE_RATIO(14.0f/8.0f);
//const float GHOST_SPEED_RATIO(39.75f/ 8.0f);
const float GHOST_SPEED(speedRatio * TILE_SIZE * LevelSpecs["ghostSpeed"][1]);
// TODO: Just got the map implemented as a field of the game class. Use that in this GetAdjDirection function.
// Maybe clean up constuctor to only have what's necesary. I feel like I don't have to pass the map every update.
std::map<std::string, glm::vec2> Ghost::ScatterTile= {
    {"redGhost", glm::vec2(25.0f, -4.0f)},
    {"pinkGhost", glm::vec2(2.0f, -4.0f)},
    {"blueGhost", glm::vec2(27.0f, 31.0f)},
    {"orangeGhost", glm::vec2(0.0f, 31.0f)}
};



std::array<float, 8> Ghost::GetScatterTimings(int level) {
    assert(level >= 1 && "ERROR: Level must be at least 1");
    if (level == 1) {
        return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f, 20.0f};
    }
    else if (level >= 2 && level <= 4) {
        return std::array<float,8>{7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 1033.0f, 0.0167f, 1000.0f};
    }
    return std::array<float,8>{5.0f, 20.0f, 5.0f, 20.0f, 5.0f, 1037.0f, 0.0167f, 1000.0f};
    
    
};

std::map<int, int> Ghost::FrightenedDurations = {
    {1, 6},
    {2, 5}
};

static glm::vec3 GetGhostColor(std::string name) {
    glm::vec3 color;
    if (name == "red" || name == "redGhost")
        color = glm::vec3(1.0f, 0.0f, 0.0f);
    else if (name == "pink" || name == "pinkGhost")
        color = glm::vec3(1.0f, 0.72f, 1.0f);
    else if (name == "blue" || name == "blueGhost")
        color = glm::vec3(0.0f, 1.0f, 1.0f);
    else if (name == "orange" || name == "orangeGhost")
        color = glm::vec3(1.0f, 0.72f, 0.325f);
    return color;
}
static glm::vec2 GetGhostHome(std::string name) {
    glm::vec2 home;
    if (name == "red" || name == "redGhost")
        home = glm::vec2(14, 11);
    else if (name == "pink" || name == "pinkGhost")
        home = glm::vec2(23, 15);
    else if (name == "blue" || name == "blueGhost")
        home = glm::vec2(23, 13);
    else if (name == "orange" || name == "orangeGhost")
        home = glm::vec2(23, 14);
    return home;
}
std::array<float, 8> Ghost::ScatterTimings = Ghost::GetScatterTimings(1);
int Ghost::FrightenedDuration = LevelSpecs["frightenedTime"][1];
float ModeTimer = Ghost::ScatterTimings[0];


Ghost::Ghost(std::string name, GameObject* player, std::vector<std::vector<int>>* map, const float tileSize, Ghost* redGhost) 
    : GameObject(
        glm::vec2(0.0f),
        glm::vec2(GHOST_SIZE_RATIO * tileSize),
        tileSize,
        std::vector<Texture2D>{ResourceManager::GetTexture("ghost")},
        1.0f,
        GetGhostColor(name),
        1.0f,
        GHOST_SPEED,
        name + std::string("Ghost")
    )
{
    std::cout << "Initial Velocity: " << this->Velocity << std::endl;
    this->TILE_SIZE = tileSize;
    this->Map = map;
    
    this->Row = GetGhostHome(this->Name).y;
    this->Col = GetGhostHome(this->Name).x;

    // if (this->Name == "redGhost") {
    //     this->Col += 2;
    // }
    glm::vec2 tileCenter = glm::vec2(this->Col, this->Row) * tileSize + tileSize/2.0f;
    this->Position = tileCenter - this->Size / 2.0f;
    this->Dir = RIGHT;
    this->AdjTargetPos = glm::vec2(this->Col, this->Row);
    this->MapTargetPos = player->Position;

    // this->AdjTargetPos = glm::vec2(this->Row, this->Col);

    this->DebugMapTarget = new GameObject(
        glm::vec2(0.0f), // position
        glm::vec2(tileSize),
        tileSize,
        std::vector<Texture2D>{ResourceManager::GetTexture("px")},
        1.0f,
        GetGhostColor(name),
        1.0f, 
        0.0f,
        name + "redMapTarget"
    );
    //Eyes(Ghost* parentGhost, float tileSize, std::vector<Texture2D> sprites);

    this->EyesObj = new Eyes(this, this->TILE_SIZE);

}

void Ghost::TurnAround(Direction currentDir, glm::vec2 currentAdjTarget) {
    this->Dir = oppositeDirection(currentDir);
    this->AdjTargetPos = currentAdjTarget + dirToVec(this->Dir);

}

void Ghost::Update(float deltaTime, PlayerObject* player, Ghost* redGhost) {
    GameObject::Update(deltaTime);

    this->EyesObj->Update(deltaTime);

    this->Row = floor(this->CenterPos.y/ (int)this->TILE_SIZE);
    this->Col = floor(this->CenterPos.x / (int)this->TILE_SIZE);


    if (CurrentMode == FRIGHTENED) {
        FrightenedTimer -= (deltaTime);
        if (FrightenedTimer <= 0.0f) {
            ExitFrightened();
        }
    }
    else if (ModeTimer >= 0.0f) {
        ModeTimer -= (deltaTime);
    }

    if (ModeTimer <= 0.0f) {
        if (ModeIdx + 1 < Ghost::ScatterTimings.size()) {
            CurrentMode = switchMode(CurrentMode);
            TurnAround(this->Dir, this->AdjTargetPos);
            std::cout << "Entered " << CurrentMode << std::endl;
            ModeTimer = Ghost::ScatterTimings[ModeIdx+1];
        }
        else {
            ModeTimer = Ghost::ScatterTimings[Ghost::ScatterTimings.size()-1];
        }        
    }

    // set map target (ghost color dependent)
    this->MapTargetPos = GetMapTarget(player->Row, player->Col, player->Facing, this->Name, Ghost::CurrentMode, redGhost);
    
    this->DebugMapTarget->Position = this->MapTargetPos * this->TILE_SIZE;
    this->DebugMapTarget->Size = glm::vec2(this->TILE_SIZE);

    // if adj target reached, get new adj target and direction towards it
    if (AdjTargetReached(glm::vec2(this->CenterPos.x, this->CenterPos.y), this->AdjTargetPos, this->TILE_SIZE)) {
        //std::cout << "adj target reached" << std::endl;
        this->AdjTargetPos = GetAdjTarget(this->Row, this->Col, this->Size, this->Dir, this->MapTargetPos, this->TILE_SIZE, this->Map);//this->Position, this->Size, this->Dir, this->MapTargetPos, tileSize, map);
        this->Dir = GetDirection(this->Row, this->Col, this->AdjTargetPos);
    }

    // move ghost in correct direction
    if (this->FreezeTimer > 0.0f) {
        this->FreezeTimer -= deltaTime;
    }
    else {
        //this->Position = Move(deltaTime, this->Position, this->Dir, this->Velocity);   
    }
}


/* Returns new position of ghost using current position, speed, and direction*/
glm::vec2 Ghost::Move(float deltaTime, glm::vec2 pos, Direction dir, float speed) {
    return pos + dirToVec(dir) * speed * deltaTime;
}

glm::vec2 Ghost::GetMapTarget(int playerRow, int playerCol, Direction playerDir, std::string ghostName, GhostMode currentMode, Ghost* redGhost) {
    //std::cout << "Inside GetMapTarget with name = " << ghostName << std::endl;

    if (currentMode == SCATTER) {
        return ScatterTile.at(ghostName);
    }

    if (ghostName == "redGhost") {
        return glm::vec2(playerCol, playerRow);
    }
    else if (ghostName == "pinkGhost") {
        glm::vec2 target = glm::vec2(playerCol, playerRow) + 4.0f * dirToVec(playerDir);
        if (playerDir == UP) // replicate overflow bug in original
            target += glm::vec2(-4.0f, 0.0f);
        return target;
    }
    else if (ghostName == "blueGhost") {
        int deltaX = redGhost->Col - playerCol;
        int deltaY = redGhost->Row - playerRow;
        glm::vec2 target = glm::vec2(playerCol - deltaX, playerRow - deltaY);
        if (playerDir == UP) // replicate overflow bug in original
            target += glm::vec2(-2.0f, -2.0f);
        return target;
    }
    else if (ghostName == "orangeGhost") {
        if (GetDistance(this->Col, this->Row, playerCol, playerRow) >= 8)
            return glm::vec2(playerCol, playerRow);

        //std::cout << EXAMPLE_INT << std::endl;
        return ScatterTile.at("orangeGhost");
    }
    std::cout << "ERROR:: INVALID GHOST NAME" << std::endl;
    return glm::vec2(0.0f);
}

/* Return if ghost has reached adjacent tile, given its center and target tile */
bool Ghost::AdjTargetReached(glm::vec2 ghostCenter, glm::vec2 adjTargetPos, float tileSize) {
    // get target coordinates in pixels from target tile
    glm::vec2 pixelTarget = (adjTargetPos * tileSize) + (tileSize/2.0f);

    // compare ghost center to pixel coordinate
    float threshold = 0.01f;
    bool xReached = abs(pixelTarget.x - ghostCenter.x) <= threshold;
    bool yReached = abs(pixelTarget.y - ghostCenter.y) <= threshold;

    // reached in both axes
    return xReached && yReached;
}

/* This function updates ghost direction and
returns adjacent target tile position */
Direction Ghost::GetDirection(int row, int col, glm::vec2 adjTargetPos) {
    float threshold = 0.001f;
    float deltaX = adjTargetPos.x - col;
    float deltaY = adjTargetPos.y - row;
    
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

void Ghost::EnterFrightened() {
    TurnAround(this->Dir, this->AdjTargetPos);
    this->PreviousModeBeforeFrightened = this->CurrentMode;
    this->CurrentMode = FRIGHTENED;
    this->Color = glm::vec3(0.129f, 0.129f, 1.0f);
    this->EyesObj->currSprite = 4;
    this->Velocity = speedRatio * TILE_SIZE * LevelSpecs["frightenedGhostSpeed"][1];
    FrightenedTimer = Ghost::FrightenedDuration;

}

void Ghost::ExitFrightened() {
    this->CurrentMode = this->PreviousModeBeforeFrightened;
    this->Color = GetGhostColor(this->Name);
    this->Velocity = speedRatio * TILE_SIZE * LevelSpecs["ghostSpeed"][1];
    std::cout << "Exit frightened" << std::endl;
}   

void Ghost::EnterEaten() {
    this->CurrentMode = EATEN;
    this->Alpha = 0.0f;
    //this->Velocity = speedRatio * TILE_SIZE * LevelSpecs["ghostSpeed"][1] * 2.0f;
    this->Velocity = speedRatio * TILE_SIZE * LevelSpecs["ghostSpeed"][1];
    std::cout << "Setting velocity to " << speedRatio * TILE_SIZE * LevelSpecs["ghostSpeed"][1] << std::endl;

}
// Remember adj target is in terms of tiles
glm::vec2 Ghost::GetAdjTarget(int row, int col, glm::vec2 ghostSize, Direction currDir, glm::vec2 mapTargetPos, float tileSize, std::vector<std::vector<int>>* map) {
    Direction dirs[4] = {LEFT, UP, RIGHT, DOWN};
    Direction randomDir;
    int tile;
    if (this->CurrentMode == FRIGHTENED) {
        randomDir = dirs[rand() % (sizeof(dirs) / sizeof(dirs[0]))];
        tile = map->at(row+dirToVec(randomDir).y).at(col+dirToVec(randomDir).x);
        //std::cout << randomDir << std::endl;

        while (dirToVec(currDir) == -dirToVec(randomDir) || tile == 0) {
            //std::cout << "Current dir is " << currDir << "But rolled opposite dir " << randomDir << "... Retrying" << std::endl;
            randomDir = dirs[rand() % (sizeof(dirs) / sizeof(dirs[0]))];
            tile = map->at(row+dirToVec(randomDir).y).at(col+dirToVec(randomDir).x);

        }

        return glm::vec2((float)col + dirToVec(randomDir).x, (float)row + dirToVec(randomDir).y);
    }

    Direction closestNodeToMapTarget;
    float minDist = tileSize * 9999.0f;
    for (Direction dir : dirs) {
        glm::vec2 dirVec = dirToVec(dir);
        if (dirVec == -dirToVec(currDir)) {
            //std::cout << "Filtered out direction (backtracking) " << dir << std::endl;
            continue;
        }
        int tile = map->at(row+dirVec.y).at(col+dirVec.x);
        if (tile == 0) {
            //std::cout << "Filtered out direction (wall)" << dir << std::endl;
            continue;
        }
        float dist = GetDistance(col + dirVec.x, row + dirVec.y, mapTargetPos.x, mapTargetPos.y);
        if (dist < minDist) {
            closestNodeToMapTarget = dir;
            minDist = dist;
        }
    }

    float adjTargetX = col + dirToVec(closestNodeToMapTarget).x;
    float adjTargetY = row + dirToVec(closestNodeToMapTarget).y;

    return glm::vec2(adjTargetX, adjTargetY);

}

float Ghost::GetDistance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2-x1, 2.0f) + pow(y2-y1,2.0f));
}


Ghost::~Ghost() {
    delete this->EyesObj;
    delete this->DebugMapTarget;
}
