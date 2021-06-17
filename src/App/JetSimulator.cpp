#include <App/JetSimulator.hpp>
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
    phongShader = make_unique<PhongShader>();
    jet = make_unique<Jet>();
    land = make_unique<Model>("assets/terrains/channeledLand/channeledLand.dae");
    camera = make_unique<ModelTrackingCamera>(jet.get(), 5, 12, glm::pi<float>()/2, getAspectRatio(), .1, 100000);
    skybox = make_unique<SkyBox>();
    effects = {false}; // initially

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

void JetSimulator::onDestroy() { }

// TODO: should be able to move mouse while hitting one of those buttons
void JetSimulator::onKeyPressed(int key, int modifierKey) {
	if (key == KEY_1) {
		effects.fog = !effects.fog;
	}
	if (key == KEY_2) {
		effects.vignette = !effects.vignette;
	}
	if (key == KEY_3) {
		effects.grayscale = !effects.grayscale;
	}
	if (key == KEY_4) {
		effects.sepia = !effects.sepia;
	}
	if (key == MOUSE_BUTTON_LEFT) {
		jet->fireMissile();
	}
}

void JetSimulator::onKeyReleased(int key, int modifierKey) {}

void JetSimulator::onUpdate(float dt) {
    jet->update(dt);
    camera->update(dt);
}

void JetSimulator::onRender() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    phongShader->use();
    phongShader->setDirLight(sun);
    phongShader->setViewPos(camera->position);
    phongShader->setProjView(camera->projection * camera->view);
	phongShader->setEffects(effects);
    jet->render(phongShader);
    land->render(phongShader);

    skybox->render(camera->projection, camera->view, glm::vec2((int)getWidth(), (int)getHeight()), effects);
}
