#include <App/App.hpp>
#include <glad/glad.h>
#include <iostream>

using namespace std;

static optional<reference_wrapper<App>> staticAppPtr = {};

inline void fatal(char* string) {
    cerr << string << "\n";
    exit(EXIT_FAILURE);
}

App::App() {
    if (staticAppPtr) {
        fatal("Can only create one app");
    }
    staticAppPtr = {*this};

    if (!glfwInit()) {
        fatal("Failed to initialize GLFW");
    }
}

App::~App() {
    if (&window.value().get()) {
        glfwDestroyWindow(&window.value().get());
    }
    glfwTerminate();
}

App& App::getApp() {
    return *staticAppPtr;
}

static float _yscroll = 0;
static void _scrollCallback(GLFWwindow* window, double dx, double dy) {
    _yscroll = dy;
}

static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        staticAppPtr.value().get().onKeyPressed(key, mods);
    } else if (action == GLFW_RELEASE) {
        staticAppPtr.value().get().onKeyReleased(key, mods);
    }
}

static void _mouseCallback(GLFWwindow* window, int key, int action, int mods) {
    _keyCallback(window, key, 0, action, mods);
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
    window = {*glfwCreateWindow(config.width, config.height, config.title.data(), monitor, nullptr)};
    if (!window) {
        glfwTerminate(); 
        fatal("Failed to create Window");
    }

    glfwSetInputMode(&window.value().get(), GLFW_CURSOR, config.cursorHidden? GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
    if (config.cursorCentered) glfwSetCursorPos(&window.value().get(), config.width/2.0, config.height/2.0);

    
    glfwSetScrollCallback(&window.value().get(), _scrollCallback);
    glfwSetKeyCallback(&window.value().get(), _keyCallback);
    glfwSetMouseButtonCallback(&window.value().get(), _mouseCallback);

    glfwMakeContextCurrent(&window.value().get());
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(&window.value().get()); 
        glfwTerminate(); 
        fatal("Failed to initialize GLAD");
    }    
}

void App::mainLoop() {
    float currentFrame,
        lastFrame = glfwGetTime(), 
        acc = 0;

    while (!glfwWindowShouldClose(&window.value().get()) && !glfwGetKey(&window.value().get(),KEY_ESCAPE)) {
        this->yscroll = _yscroll;
        _yscroll = 0;

        currentFrame = glfwGetTime();
        acc += currentFrame - lastFrame;

        while (acc >= updateTime) {
            this->onUpdate(updateTime);
            acc -= updateTime;
        }
        this->onRender();

        glfwSwapBuffers(&window.value().get());
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
    return glfwGetKey(&window.value().get(), key);
}

glm::vec2 App::getMousePos() {
    double x, y;
    glfwGetCursorPos(&window.value().get(), &x, &y);
    return glm::vec2(x, y);
}

void App::close() {
    glfwSetWindowShouldClose(&window.value().get(), GLFW_TRUE);
}

int App::getWidth() {
    int width, height;
    glfwGetWindowSize(&window.value().get(), &width, &height);
    return width;
}

int App::getHeight() {
    int width, height;
    glfwGetWindowSize(&window.value().get(), &width, &height);
    return height;
}

float App::getAspectRatio() {
    int width, height;
    glfwGetWindowSize(&window.value().get(), &width, &height);
    return (float)width/height;
}

float App::getScroll() {
    float temp = this->yscroll;
    this->yscroll = 0;
    return temp;
}
