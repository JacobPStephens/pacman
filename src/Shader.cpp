#include "Shader.h"
#include <iostream>


Shader& Shader::Use() {
    glUseProgram(this->shaderProgramID);
    return *this;
}

void Shader::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
    
    // 1. Using shader codes, create shader objects
    unsigned int vertexShader, fragmentShader, geometryShader;

    // create empty shader object and store into vertexShader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // assign shader object contents to vertex shader code 
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // compile shader and print errors, if any
    glCompileShader(vertexShader);
    checkCompilationErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    checkCompilationErrors(fragmentShader, "FRAGMENT");

    if (geometrySource != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, NULL);
        glCompileShader(geometryShader);
        checkCompilationErrors(geometryShader, "GEOMETRY");
    }

    // 2. using shader objects, create a shader program
    this->shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    if (geometrySource != nullptr) {
        glAttachShader(shaderProgramID, geometryShader);
    }
    // link program and print errors, if any
    glLinkProgram(shaderProgramID);
    checkCompilationErrors(this->shaderProgramID, "PROGRAM");

    // delete shader objects, since they are now part of shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometrySource != nullptr) {
        glDeleteShader(geometryShader);
    }

}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->shaderProgramID, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->shaderProgramID, name), value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->shaderProgramID, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->shaderProgramID, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->shaderProgramID, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->shaderProgramID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->shaderProgramID, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->shaderProgramID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgramID, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompilationErrors(unsigned int object, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);    
            std::cout << "ERROR::SHADER::COMPILATION_FAILED: Type: " << type << "\n"
                      << infoLog << "\n -------------------------------------"
                      << std::endl;
        };
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINK_FAILED: Type: " << type << "\n"
                      << infoLog << "\n -------------------------------------"
                      << std::endl;
        }
    }
}

