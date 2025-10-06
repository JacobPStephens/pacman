#ifndef DIRECTION_H
#define DIRECTION_H

#include <glm/vec2.hpp>

enum Direction {
    NONE,
    RIGHT,
    UP,
    LEFT,
    DOWN
};

glm::vec2 dirToVec(Direction direction);

Direction oppositeDirection(Direction direction);

#endif