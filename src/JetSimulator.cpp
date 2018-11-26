#include "JetSimulator.hpp"
#include <glad/glad.h>

Application::Config JetSimulator::getConfig() {
    Config c;
    c.title = "JetSimulator";
    c.width = 800;
    c.height = 600;
    c.isFullscreen = false;
    c.resizable = false;
    c.updateRate = 100;

    c.glMajorVersion = 3;
    c.glMinorVersion = 3;
    return c;
}

void JetSimulator::onCreate() {

}

void JetSimulator::onDestroy() {

}

void JetSimulator::onUpdate(double dt) {

}

void JetSimulator::onDraw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
}
