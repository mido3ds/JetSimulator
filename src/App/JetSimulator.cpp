#include "JetSimulator.hpp"
#include <glad/glad.h>
#include <glm/ext.hpp>

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
    c.samples = 4;

    c.glMajorVersion = 3;
    c.glMinorVersion = 3;
    return c;
}

void JetSimulator::onCreate() {
    phongShader = new PhongShader();
    jet = new Jet();
    land = new Model("assets/terrains/channeledLand/channeledLand.dae");
    camera = new ModelTrackingCamera(jet, 5, 12, glm::pi<float>()/2, getAspectRatio(), .1, 100000);
    skybox = new SkyBox();
	useFog = false; // initially
	useVignette = false; // initially

    phongShader->use();
	phongShader->setUniform(phongShader->getUniformLocation("uResolution"), glm::vec2((int)getWidth(), (int)getHeight()));
    jet->load();
    land->load();
    skybox->load();

    jet->pos.z = 50.0f;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void JetSimulator::onDestroy() {
    delete phongShader;
    delete camera;
    delete jet;
    delete land;
    delete skybox;
}

void JetSimulator::onKeyPressed(int key, int modifierKey) {
	if (key == KEY_1) {
		useFog = !useFog;
	}
	else if (key == KEY_2) {
		useVignette = !useVignette;
	}
}

void JetSimulator::onKeyReleased(int key, int modifierKey) {}

void JetSimulator::onUpdate(float dt) {
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
	//skybox->switchFog(!useFog);
	phongShader->switchFog(useFog);
	phongShader->switchVignette(useVignette);
    jet->draw(*phongShader);
    land->draw(*phongShader);

    skybox->draw(camera->projection, camera->view);
}
