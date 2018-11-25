#ifndef APPLICATION_HPP
#define APPLICATION_HPP

//Always include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Scene;

class Application {
private:
    GLFWwindow* window;
    Scene* currentScene = nullptr;

    //Delete copy constructor and assignment operation
    Application(Application const &) = delete;
    Application & operator =(Application const &) = delete;

public:
    Application(const char* title, int width, int height, bool isFullscreen = false);
    ~Application();
    void setScene(Scene* scene);
    void run();

    GLFWwindow* getWindow(){return window; }
};

#endif