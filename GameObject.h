#ifndef CLASS_GAMEOBJECT_H
#define CLASS_GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Texture2D.h>
#include <SpriteRenderer.h>

class GameObject {
    public:
        glm::vec2 Position, Size, Velocity;
        glm::vec3 Color;

        std::string Name;

        float Rotation;
        bool IsSolid;
        bool Destroyed;

        // render state
        float Lifespan;
        float CycleLength;
        std::vector<Texture2D> Sprites;

        // constructors
        GameObject();
        GameObject(glm::vec2 position, glm::vec2 size, std::vector<Texture2D> sprites, float cycleLength, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f), std::string name = "");

        void Update(float dt);
        // draw sprite (confused about virtual)
        virtual void Draw(SpriteRenderer &renderer);
};

#endif