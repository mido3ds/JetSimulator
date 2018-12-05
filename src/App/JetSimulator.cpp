#include "JetSimulator.hpp"
#include <glad/glad.h>
#include <glm/gtc/constants.hpp>

App::Config JetSimulator::getConfig() {
    Config c;
    c.title = "JetSimulator";
    c.width = 800;
    c.height = 600;
    c.isFullscreen = false;
    c.resizable = false;
    c.cursorHidden = true;
    c.cursorCentered = true;
    c.updateRate = 100;

    c.glMajorVersion = 3;
    c.glMinorVersion = 3;
    return c;
}

void JetSimulator::onCreate() {
    phongShader = new PhongShader();
    jet = new Jet();
    camera = new ModelTrackingCamera(jet, 5, glm::pi<float>()/2, getAspectRatio(), .1, 1000);

    phongShader->use();
    jet->load();
}

void JetSimulator::onDestroy() {
    delete phongShader;
    delete camera;
    delete jet;
}

void JetSimulator::onUpdate(double dt) {
    jet->update(dt);
    camera->update(dt);
}

void JetSimulator::onDraw() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    phongShader->use();
    phongShader->setDirLight(sun);
    phongShader->setViewPos(camera->position);
    phongShader->setProjView(camera->projection * camera->view);
    jet->draw(*phongShader);
}
