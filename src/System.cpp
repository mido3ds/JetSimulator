#include <System.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string_view>
#include <functional>
#include <optional>
#include <glad/glad.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <Shaders/PhongShader.hpp>
#include <Cameras/ModelTrackingCamera.hpp>
#include <Models/Jet.hpp>
#include <Scene/Light.hpp>
#include <Shaders/Effects.hpp>
#include <Models/SkyBox.hpp>
#include <JetSimulator.hpp>
#include <string_view>

inline void fatal(string_view string) {
    cerr << string << "\n";
    exit(1);
}

// forward declarations
namespace System::Input {
    static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void _mouseCallback(GLFWwindow* window, int key, int action, int mods);
    static void _scrollCallback(GLFWwindow* window, double dx, double dy);
}

namespace System {
    GLFWwindow* _window;
    JetSimulator* _sim;

    namespace Config {
        constexpr auto title           = "JetSimulator";
        
        constexpr auto width           = 800;
        constexpr auto height          = 600;

        constexpr auto isFullscreen    = false;
        constexpr auto resizable       = false;
        constexpr auto cursorHidden    = true;
        constexpr auto cursorCentered  = true;

        constexpr auto updatesPerSec   = 100;
        constexpr auto samples         = 4;

        constexpr auto glMajorVersion  = 3;
        constexpr auto glMinorVersion  = 3;
    };

    namespace Graphics {
        int _width, _height;

        int getWidth() {
            glfwGetWindowSize(_window, &_width, &_height);
            return _width;
        }

        int getHeight() {
            glfwGetWindowSize(_window, &_width, &_height);
            return _height;
        }

        float getAspectRatio() {
            glfwGetWindowSize(_window, &_width, &_height);
            return (float)_width/_height;
        }

        inline void _init() {
            if (!glfwInit()) {
                fatal("Failed to initialize GLFW");
            }

            // configs
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::glMajorVersion);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::glMinorVersion);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, Config::resizable? GLFW_TRUE:GLFW_FALSE);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
            glfwWindowHint(GLFW_RED_BITS, 8);
            glfwWindowHint(GLFW_GREEN_BITS, 8);
            glfwWindowHint(GLFW_BLUE_BITS, 8);
            glfwWindowHint(GLFW_ALPHA_BITS, 8);
            glfwWindowHint(GLFW_DEPTH_BITS, 24);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);
            glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
            glfwWindowHint(GLFW_SAMPLES, Config::samples);

            // monitor
            auto monitor = Config::isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
            _window = glfwCreateWindow(Config::width, Config::height, Config::title, monitor, nullptr);
            if (!_window) {
                glfwTerminate(); 
                fatal("Failed to create Window");
            }

            // cursor
            glfwSetInputMode(_window, GLFW_CURSOR, Config::cursorHidden? GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
            if (Config::cursorCentered) { 
                glfwSetCursorPos(_window, Config::width/2.0, Config::height/2.0); 
            }

            // callbacks
            glfwSetScrollCallback(_window, System::Input::_scrollCallback);
            glfwSetKeyCallback(_window, System::Input::_keyCallback);
            glfwSetMouseButtonCallback(_window, System::Input::_mouseCallback);

            // GLAD
            glfwMakeContextCurrent(_window);
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                glfwDestroyWindow(_window); 
                glfwTerminate(); 
                fatal("Failed to initialize GLAD");
            }    
        }

        inline void _destroy() {
            glfwDestroyWindow(_window);
            glfwTerminate();
        }
    }

    namespace Input {
        Scroll _scroll = {0};
        MousePos _pos = {0};

        static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) {
                _sim->onKeyPressed(Key(key), Key(mods));
            } else if (action == GLFW_RELEASE) {
                _sim->onKeyReleased(Key(key), Key(mods));
            }
        }

        static void _mouseCallback(GLFWwindow* window, int key, int action, int mods) {
            _keyCallback(System::_window, key, 0, action, mods);
        }

        static void _scrollCallback(GLFWwindow* window, double dx, double dy) {
            _scroll.dx = dx;
            _scroll.dy = dy;
        }

        bool isKeyPressed(Key key) {
            return glfwGetKey(System::_window, int(key));
        }

        Scroll getScroll() {
            return _scroll;
        }

        MousePos getMousePos() {
            glfwGetCursorPos(System::_window, &_pos.x, &_pos.y);
            return _pos;
        }
    }

    static void _errorCallback(int code, char const* desc) {
        fatal(string(desc) + string(", Error code = ") + to_string(code));
    }

    inline void _init() {
        glfwSetErrorCallback(_errorCallback);
        Graphics::_init();
        _sim = new JetSimulator();
    }

    inline void _mainLoop() {
        float currentFrame,
            lastFrame = glfwGetTime(), 
            acc = 0,
            updateTime = (float)1.0/Config::updatesPerSec;

        while (!glfwWindowShouldClose(System::_window) && !System::Input::isKeyPressed(Key::ESCAPE)) {
            currentFrame = glfwGetTime();
            acc += currentFrame - lastFrame;

            while (acc >= updateTime) {
                _sim->onUpdate(updateTime);
                acc -= updateTime;
            }
            _sim->onRender();

            glfwSwapBuffers(System::_window);
            glfwPollEvents();

            lastFrame = currentFrame;
            Input::_scroll = {0};
        }
    }

    inline void _destroy() {
        delete _sim;
        Graphics::_destroy();
    }

    void close() {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }
}

int main() {
    System::_init();
    System::_mainLoop();
    System::_destroy();

    return 0;
}
