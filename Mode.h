#ifndef MODE_H
#define MODE_H

enum GhostMode {
    SCATTER,
    CHASE,
    FRIGHTENED,
    EATEN
};

enum PlayerMode {
    NORMAL,
    FEASTING,
    EATING,
    DYING
};

// inline allows the linker to be okay with multiple definitions
inline GhostMode switchMode(GhostMode m) {
    if (m == SCATTER) 
        return CHASE;
    return SCATTER;
}

#endif