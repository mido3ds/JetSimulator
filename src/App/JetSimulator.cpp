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

/* color texture and depth-stencil renderbuffer with given width and height */
class Framebuffer {
protected:
    GLuint fbo, colorTexture, rbo;

    Framebuffer(Framebuffer const &) = delete;
    Framebuffer & operator =(Framebuffer const &) = delete;
public:
    Framebuffer(int width, int height) {
        // create color texture
        glGenTextures(1, &colorTexture);
        glBindTexture(GL_TEXTURE_2D, colorTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // create depth-stencil renderbuffer
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // create framebuffer
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);  

            // attach them
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0); 
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

            // check
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    }

    ~Framebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  

        glDeleteTextures(1, &colorTexture);
        glDeleteRenderbuffers(1, &rbo);
        glDeleteFramebuffers(1, &fbo);  
    }

    void bind() const {glBindFramebuffer(GL_FRAMEBUFFER, fbo);}
    static void bindDefault() {glBindFramebuffer(GL_FRAMEBUFFER, NULL);}

    constexpr GLuint getID() const {return fbo;}
    constexpr GLuint getColorBufferID() const {return colorTexture;}
    constexpr GLuint getDepthStencilBufferID() {return rbo;}
};

#include <sstream>
#define STR(n) #n
class Effect {
protected:
    Shader shader;
    GLuint vao, vbo;

public:
    Effect(const std::string& func) {
        std::stringstream ss;
        ss << "#version 330 core\n" 
            << "in VS_OUT {vec2 texCoords;} from_vs;\n"
            << "out vec4 outFragColor;\n"
            << "uniform sampler2D uTexture;\n";
        ss << '\n' << func << "\n\n";
        ss << "void main() {outFragColor = apply(uTexture, from_vs.texCoords);}";

        printf("%s\n", ss.str().c_str());

        shader.attachFile("assets/shaders/screen.vs.glsl", GL_VERTEX_SHADER);
        shader.attach(ss.str(), GL_FRAGMENT_SHADER);
        shader.link();

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
                int dummyBuffer[6];
                glBufferData(GL_ARRAY_BUFFER, sizeof(dummyBuffer), &dummyBuffer, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Effect() {
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    inline Shader* getShader() {return &shader;}

    void apply(GLuint textureID) {
        glDisable(GL_DEPTH_TEST);
            shader.use();
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }
};

class Renderer {
protected:
    Framebuffer screenFramebuffer;
    Shader screenShader;
    GLuint vao, vbo;

public:
    Renderer(int width, int height) :screenFramebuffer(width, height) {
        screenShader.attachFile("assets/shaders/screen.vs.glsl", GL_VERTEX_SHADER);
        screenShader.attachFile("assets/shaders/screen.fs.glsl", GL_FRAGMENT_SHADER);
        screenShader.link();

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
                int dummyBuffer[6];
                glBufferData(GL_ARRAY_BUFFER, sizeof(dummyBuffer), &dummyBuffer, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Renderer() {
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void startFrame() {
        screenFramebuffer.bind();
    }
    
    void endFrame() {
        glDisable(GL_DEPTH_TEST);
            Framebuffer::bindDefault();
            screenShader.use();
            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, screenFramebuffer.getColorBufferID());
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    inline void applyEffect(Effect* eff) {
        eff->apply(screenFramebuffer.getColorBufferID());
    }
};

static Renderer* renderer;
static Effect* inverseEffect;
static Effect* grayscaleEffect;
static Effect* sepiaEffect;
static Effect* vignetteEffect;
void JetSimulator::onCreate() {
    phongShader = new PhongShader();
    jet = new Jet();
    land = new Model("assets/terrains/channeledLand/channeledLand.dae");
    camera = new ModelTrackingCamera(jet, 5, 12, glm::pi<float>()/2, getAspectRatio(), .1, 100000);
    skybox = new SkyBox();
	useFog = false; // initially
	useVignette = false; // initially
	useGrayscale = false; // initially
	useSepia = false; // initially

    phongShader->use();
	phongShader->setUniform(phongShader->getUniformLocation("uResolution"), glm::vec2((int)getWidth(), (int)getHeight()));
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

    vignetteEffect = new Effect(STR(
        const float OUTER_RADIUS = 0.65;
        const float INNER_RADIUS = 0.4;
        const float INTENSITY = 1;
        const vec2 uResolution = vec2(800, 600);

        vec4 apply(sampler2D tex, vec2 coords) {
            vec3 color = vec3(texture(tex, coords));

            float len = length(gl_FragCoord.xy/uResolution - 0.5);
            float vignette = smoothstep(OUTER_RADIUS, INNER_RADIUS, len); 

            return vec4(mix(color, color * vignette, INTENSITY), 1);
        }
    ));
}

void JetSimulator::onDestroy() {
    delete renderer;
    delete inverseEffect;
    delete grayscaleEffect;
    delete sepiaEffect;
    delete vignetteEffect;
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
	skybox->switchVignette(false);
	skybox->switchGrayscale(false);
	skybox->switchSepia(false);
    skybox->draw(camera->projection, camera->view, glm::vec2((int)getWidth(), (int)getHeight()));

    if (useGrayscale) renderer->applyEffect(grayscaleEffect);
    if (useSepia) renderer->applyEffect(sepiaEffect);
    if (useVignette) renderer->applyEffect(vignetteEffect);

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