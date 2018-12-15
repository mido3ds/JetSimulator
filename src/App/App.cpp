#include "App.hpp"
#include <glad/glad.h>
#include <Windows.h>

static App* staticAppPtr = nullptr;

inline void fatal(char* string) {
    MessageBox(NULL, TEXT(string), TEXT("Error"), MB_OK|MB_ICONERROR);
    exit(EXIT_FAILURE);
}

App::App() {
    if (staticAppPtr) {
        fatal("Can only create one app");
    }
    staticAppPtr = this;

    if (!glfwInit()) {
        fatal("Failed to initialize GLFW");
    }
}

App::~App() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

App* App::getApp() {
    return staticAppPtr;
}

static float _yscroll = 0;
static void _scrollCallback(GLFWwindow* window, double dx, double dy) {
    _yscroll = dy;
}

static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        staticAppPtr->onKeyPressed(key, mods);
    } else if (action == GLFW_RELEASE) {
        staticAppPtr->onKeyReleased(key, mods);
    }
}

void App::createWindow() {
    Config config = getConfig();

    updateTime = (float)1.0/config.updateRate;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable? GLFW_TRUE:GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_SAMPLES, config.samples);

    GLFWmonitor *monitor = config.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(config.width, config.height, config.title, monitor, nullptr);
    if (!window) {
        glfwTerminate(); 
        fatal("Failed to create Window");
    }

    glfwSetInputMode(window, GLFW_CURSOR, config.cursorHidden? GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
    if (config.cursorCentered) glfwSetCursorPos(window, config.width/2.0, config.height/2.0);

    
    glfwSetScrollCallback(window, _scrollCallback);
    glfwSetKeyCallback(window, _keyCallback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window); 
        glfwTerminate(); 
        fatal("Failed to initialize GLAD");
    }    
}

void App::mainLoop() {
    float currentFrame,
        lastFrame = glfwGetTime(), 
        acc = 0;

    while (!glfwWindowShouldClose(window) && !glfwGetKey(window,KEY_ESCAPE)) 
	{
        this->yscroll = _yscroll;
        _yscroll = 0;

        currentFrame = glfwGetTime();
        acc += currentFrame - lastFrame;

        while (acc >= updateTime) {
            this->onUpdate(updateTime);
            acc -= updateTime;
        }
        this->onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastFrame = currentFrame;

        this->yscroll = 0;
    }
}

void App::run() {
    createWindow();

    this->onCreate();
    mainLoop();
    this->onDestroy();
}

bool App::isKeyPressed(int key) {
    return glfwGetKey(window, key);
}

glm::vec2 App::getMousePos() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return glm::vec2(x, y);
}

void App::close() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int App::getWidth() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return width;
}

int App::getHeight() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return height;
}

float App::getAspectRatio() {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return (float)width/height;
}

float App::getScroll() {
    float temp = this->yscroll;
    this->yscroll = 0;
    return temp;
}