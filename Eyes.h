#ifndef EYES_CLASS_H
#define EYES_CLASS_H

#include "GameObject.h"
#include "Ghost.h"
#include "Direction.h"

class Eyes : public GameObject {

    public:
        Eyes(Ghost* parentGhost, float tileSize, std::vector<Texture2D> sprites);
        void Update(float dt);
        void SetSprite(Direction direction);

        int currSprite;
        Ghost* parentGhost;
};

#endif