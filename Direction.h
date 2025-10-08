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

inline glm::vec2 dirToVec(Direction direction) {
    switch (direction) {
        case RIGHT:
            return glm::vec2(1.0f, 0.0f);
        case UP:
            return glm::vec2(0.0f, -1.0f);
        case LEFT:
            return glm::vec2(-1.0f, 0.0f);
        case DOWN:
            return glm::vec2(0.0f, 1.0f);
        default:
            return glm::vec2(0.0f);
    }
};

inline Direction oppositeDirection(Direction direction) {
    switch (direction) {
        case RIGHT:
            return LEFT;
        case UP:
            return DOWN;
        case LEFT:
            return RIGHT;
        case DOWN:
            return UP;
        default:
            return RIGHT;
    }
}

#endif