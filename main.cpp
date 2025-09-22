
#include "Game.h"
#include "ResourceManager.h"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const int RESOLUTION_SCALE = 4;
const unsigned int SCREEN_WIDTH = 224 * RESOLUTION_SCALE;
const unsigned int SCREEN_HEIGHT = 248 * RESOLUTION_SCALE;

Game* Pacman = new Game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[]) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pac-man", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers 
    // (don't know what this does, looks like error checking
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // opengl configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // dont know what this does

    // init game
    Pacman->Init();

    // dt setup
    float deltaTime = 0.0;
    float prevTime = 0.0;

    // opengl loop
    while (!glfwWindowShouldClose(window)) {

        // calc delta time
        float currTime = glfwGetTime();
        deltaTime = currTime - prevTime;
        prevTime = currTime;
        glfwPollEvents();

        // user input
        Pacman->ProcessInput(deltaTime);

        // update 
        Pacman->Update(deltaTime);
        
        // render
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        Pacman->Render();

        glfwSwapBuffers(window);

    }
    // delete all resources
    ResourceManager::Clear();
    
    // delete pointer breakout to avoid segfault
    delete Pacman;

    // terminate opengl
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {


    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key >= 0 && key < 1024) {

        if (action == GLFW_PRESS) {
            Pacman->Keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            Pacman->Keys[key] = false;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // can respond to switch of viewport
    glViewport(0, 0, width, height);
}