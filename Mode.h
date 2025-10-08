#ifndef MODE_H
#define MODE_H

enum Mode {
    SCATTER,
    CHASE
};

inline Mode switchMode(Mode m) {
    if (m == SCATTER) 
        return CHASE;
    return SCATTER;
}

#endif