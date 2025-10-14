#ifndef LEVEL_SPECS_H
#define LEVEL_SPECS_H
#include <map>
#include <string>

inline const float RESOLUTION_SCALE = 3.0f;
inline const float TILE_SIZE = 8.0f * RESOLUTION_SCALE;
inline const float speedRatio = 9.469697025f;

inline std::map<std::string, std::map<int, float>> LevelSpecs = 
{
    {"playerSpeed",  {
        {1, 0.80f},
        {2, 0.90f},
    }},
    {"ghostSpeed",  {
        {1, 0.75f},
        {2, 0.80f},
    }},
    {"frightenedGhostSpeed",  {
        {1, 0.50f},
        {2, 0.55f},
    }},
    {"frightenedTime",  {
        {1, 6},
        {2, 5},
    }},

};



#endif