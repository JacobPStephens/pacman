#ifndef EYES_CLASS_H
#define EYES_CLASS_H

#include "GameObject.h"
#include "ResourceManager.h"
#include "Direction.h"

class Ghost;

class Eyes : public GameObject {

    public:
        Eyes(Ghost* parentGhost, float tileSize);
        void Update(float dt);
        void SetSprite(Direction direction);

        int currSprite;
        Ghost* parentGhost;
};

#endif