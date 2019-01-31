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

class ScreenShader : public Shader {
public:
    ScreenShader() :Shader() {
        attach("assets/shaders/screen.vs.glsl", GL_VERTEX_SHADER);
        attach("assets/shaders/screen.fs.glsl", GL_FRAGMENT_SHADER);
        link();

        use();
        setUniform(getUniformLocation("uScreenTexture"), 0);
    }
};

static ScreenShader* screen;
static void createScreenShader() {
    screen = new ScreenShader();
}

static void deleteScreenShader() {
    delete screen;
}

/* load to gpu an array of positions and texCoords of a quad */
class QuadBuffer {
protected:
    GLuint vao, vbo;

    QuadBuffer(QuadBuffer const &) = delete;
    QuadBuffer & operator =(QuadBuffer const &) = delete;
public:
    QuadBuffer() {
        float quadVertices[] = { 
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~QuadBuffer() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void bind() const {
        glBindVertexArray(vao);
    }

    constexpr GLuint getID() const {return vao;}
};

static QuadBuffer* quad;
static void createQuadBuffer() {
    quad = new QuadBuffer();
}

static void deleteQuadBuffer() {
    delete quad;
}

/* color texture and depth-stencil renderbuffer with given width and height */
class Framebuffer {
protected:
    GLuint fbo, colorTexture, rbo;
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
            GLint type;
            glGetFramebufferAttachmentParameteriv(
                GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &type);
            assert(type == colorTexture);
            glGetFramebufferAttachmentParameteriv(
                GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &type);
            assert(type == rbo);

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

static Framebuffer* secondry;
static void initSecondryFrameBuffer(int width, int height) {
    secondry = new Framebuffer(width, height);
}

static void useSecondryFrameBuffer() {
    secondry->bind();
}

static void deleteSecondryFrameBuffer() {
    delete secondry;
}

static void drawOnMainFrameBuffer() {
    glDisable(GL_DEPTH_TEST);
        Framebuffer::bindDefault();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        screen->use();
        quad->bind();
        glBindTexture(GL_TEXTURE_2D, secondry->getColorBufferID());
        glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}

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

    initSecondryFrameBuffer(getWidth(), getHeight());
    createScreenShader();
    createQuadBuffer();

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void JetSimulator::onDestroy() {
    deleteQuadBuffer();
    deleteScreenShader();
    deleteSecondryFrameBuffer();

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
    useSecondryFrameBuffer();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    phongShader->use();
    phongShader->setDirLight(sun);
    phongShader->setViewPos(camera->position);
    phongShader->setProjView(camera->projection * camera->view);
	phongShader->switchFog(useFog);
	phongShader->switchGrayscale(useGrayscale);
	phongShader->switchSepia(useSepia);
	phongShader->switchVignette(useVignette);
    jet->draw(*phongShader);
    land->draw(*phongShader);

	skybox->switchFog(useFog);
	skybox->switchVignette(useVignette);
	skybox->switchGrayscale(useGrayscale);
	skybox->switchSepia(useSepia);
    skybox->draw(camera->projection, camera->view, glm::vec2((int)getWidth(), (int)getHeight()));

    drawOnMainFrameBuffer();
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