#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H
#include <map>
#include <string>
#include <glad/glad.h>

#include "Texture2D.h"
#include "Shader.h"


class ResourceManager {
    public:
        // define maps (key = name, value = Shader/Texture object)
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;
        // loads and generates shader program from vertex, fragment, (and geometries) shader's source code
        static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        // gets a stored shader
        static Shader& GetShader(std::string name);
        // loads and generates texture from file
        static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
        // gets stored texture
        static Texture2D& GetTexture(std::string name);
        // de-allocates all loaded resources
        static void Clear();
    private:
        // empty private constructor because we don't want any resource manager objects
        ResourceManager() {}; 
        // I don't get how these are different from the public methods
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static Texture2D loadTextureFromFile(const char* file, bool alpha);

};


#endif
