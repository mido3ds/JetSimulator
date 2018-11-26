#pragma once
#include <GLFW/glfw3.h>

class Application {
private:
    GLFWwindow* window;
    double updateTime;

    void createWindow();
    void mainLoop();

    Application(Application const &) = delete;
    Application & operator =(Application const &) = delete;
public:
    struct Config {
        char* title;
        int width, height;
        bool isFullscreen;
        bool resizable;

        int glMajorVersion;
        int glMinorVersion;
        int updateRate; // update per second
    };

    Application();
    ~Application();

    void run();
    void close();

    bool isKeyPressed(int key);
    bool isLMousePressed();
    bool isRMousePressed();
    int mouseX();
    int mouseY();

    virtual Config getConfig()=0;
    virtual void onCreate()=0;
    virtual void onDestroy()=0;
    virtual void onUpdate(double dt)=0;
    virtual void onDraw()=0;
};