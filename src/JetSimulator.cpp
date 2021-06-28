#include <JetSimulator.hpp>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include <System.hpp>

JetSimulator::JetSimulator() {
    phongShader = make_unique<PhongShader>();
    jet = make_unique<Jet>();
    land = make_unique<Model>("assets/terrains/channeledLand/channeledLand.dae");
    camera = make_unique<ModelTrackingCamera>(*jet.get(), 5, 12, glm::pi<float>()/2, System::Graphics::getAspectRatio(), .1, 100000);
    skybox = make_unique<SkyBox>();
    effects = {false}; // initially

    phongShader->use();
	phongShader->setUniform(phongShader->getUniformLocation("uResolution"), glm::vec2(System::Graphics::getWidth(), System::Graphics::getHeight()));
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

// TODO: should be able to move mouse while hitting one of those buttons
void JetSimulator::onKeyPressed(Key key, Key modifierKey) {
    switch (key) {
    case Key::NUMBER_1:
		effects.fog = !effects.fog;
        break;
    case Key::NUMBER_2:
		effects.vignette = !effects.vignette;
        break;
    case Key::NUMBER_3:
		effects.grayscale = !effects.grayscale;
        break;
    case Key::NUMBER_4:
		effects.sepia = !effects.sepia;
        break;
    case Key::MOUSE_BUTTON_LEFT:
		jet->fireMissile();
        break;
    default: break;   
    }
}

void JetSimulator::onKeyReleased(Key key, Key modifierKey) {}

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
    jet->render(*phongShader.get());
    land->render(*phongShader.get());

    skybox->render(camera->projection, camera->view, glm::vec2(System::Graphics::getWidth(), System::Graphics::getHeight()), effects);
}
