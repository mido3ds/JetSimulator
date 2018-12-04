#include "AbstractApp.hpp"
#include <glad/glad.h>
#include <iostream>
#include <exception>

AbstractApp* staticAppPtr = nullptr;

AbstractApp::AbstractApp() {
    if (staticAppPtr) throw std::exception("cant create two AbstractApps");
    staticAppPtr = this;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
}

AbstractApp::~AbstractApp() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

AbstractApp* AbstractApp::getApp() {
    return staticAppPtr;
}

void AbstractApp::createWindow() {
    Config config = getConfig();

    updateTime = (double)1.0/config.updateRate;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable? GLFW_TRUE:GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 0);    
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    GLFWmonitor *monitor = config.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(config.width, config.height, config.title, monitor, nullptr);
    if (!window) {
        std::cerr << "Failed to create Window" << std::endl;
        glfwTerminate(); 
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window); 
        glfwTerminate(); 
        exit(EXIT_FAILURE);
    }    
}

void AbstractApp::mainLoop() {
    double currentFrame,
        lastFrame = glfwGetTime(), 
        acc = 0;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        acc += currentFrame - lastFrame;

        while (acc >= updateTime) {
            this->onUpdate(updateTime);
            acc -= updateTime;
        }
        this->onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastFrame = glfwGetTime();
    }
}

void AbstractApp::run() {
    createWindow();

    this->onCreate();
    mainLoop();
    this->onDestroy();
}

bool AbstractApp::isKeyPressed(int key) {
    return glfwGetKey(window, key);
}

glm::vec2 AbstractApp::getMousePos() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return glm::vec2(x, y);
}

void AbstractApp::close() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}