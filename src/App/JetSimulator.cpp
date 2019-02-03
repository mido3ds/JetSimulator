#include <cstdio>
#include <glad/glad.h>
#include <glm/ext.hpp>
#include "JetSimulator.hpp"

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
    c.glDebug = true;
    return c;
}

void JetSimulator::onCreate() {
    phongShader = new PhongShader();
    jet = new Jet();
    land = new Model("assets/terrains/channeledLand/channeledLand.dae");
    camera = new ModelTrackingCamera(jet, 5, 12, glm::pi<float>()/2, getAspectRatio(), .1, 100000);
    skybox = new SkyBox();

    phongShader->use();
    jet->load();
    land->load();
    skybox->load();

    jet->pos.z = 50.0f;

    renderer = new Renderer(getWidth(), getHeight());

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    inverseEffect = new Effect(STR(
        vec4 apply(sampler2D tex, vec2 coords) {
            return vec4(vec3(1-texture(tex, coords)), 1);
        }
    ));

    grayscaleEffect = new Effect(STR(
        vec4 apply(sampler2D tex, vec2 coords) {
            vec4 color = texture(tex, coords);
            float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
            return vec4(average, average, average, 1.0);
        }
    ));

    sepiaEffect = new Effect(STR(
        vec4 apply(sampler2D tex, vec2 coords) {
            vec4 color = texture(tex, coords);
            float sepia_red = color.r *0.393f + color.g *0.769f + color.b *0.189f ;
            float sepia_green = color.r *0.349f + color.g *0.686f + color.b *0.168f ;
            float sepia_blue = color.r *0.272f + color.g *0.534f + color.b *0.131f ;	
            return vec4(sepia_red, sepia_green, sepia_blue, 1);
        }
    ));

    vignetteEffect = new VignetteEffect(0.4f, 0.65f, 1.0f, getWidth(), getHeight());
    
    blurEffect = new BlurEffect();
}

void JetSimulator::onDestroy() {
    delete renderer;
    delete inverseEffect;
    delete grayscaleEffect;
    delete sepiaEffect;
    delete vignetteEffect;
    delete blurEffect;
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
	if (key == KEY_2) {
		useVignette = !useVignette;
	}
	if (key == KEY_3) {
		useGrayscale = !useGrayscale;
	}
	if (key == KEY_4) {
		useSepia = !useSepia;
	}
    if (key == KEY_5) {
        useBlur = !useBlur;
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

void JetSimulator::onDraw() {
    renderer->startFrame();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    phongShader->use();
    phongShader->setDirLight(sun);
    phongShader->setViewPos(camera->position);
    phongShader->setProjView(camera->projection * camera->view);
	phongShader->switchFog(useFog);
    jet->draw(*phongShader);
    land->draw(*phongShader);

	skybox->switchFog(useFog);
    skybox->draw(camera->projection, camera->view, glm::vec2((int)getWidth(), (int)getHeight()));

    if (useGrayscale) renderer->applyEffect(grayscaleEffect);
    if (useSepia) renderer->applyEffect(sepiaEffect);
    if (useVignette) renderer->applyEffect(vignetteEffect);
    if (useBlur) renderer->applyEffect(blurEffect);

    renderer->endFrame();
}

void JetSimulator::onOpenglDebug(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, std::string message
    ) {
    printf("opengl: %s\n", message.c_str());
}

void JetSimulator::onError(int code, std::string msg) {
    printf("wndow error: %s\n", msg.c_str());
}