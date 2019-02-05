#pragma once
#include "Framebuffer.h"
#include "Effect.h"

class Renderer {
protected:
    Framebuffer screenFramebuffer;
    Shader screenShader;
    GLuint vao, vbo;
    Transition* transition = nullptr;

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
        if (transition) applyEffect(transition);

        glDisable(GL_DEPTH_TEST);
            Framebuffer::bindDefault();
            screenShader.use();
            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, screenFramebuffer.getColorBufferID());
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
    }

    void applyEffect(Effect* eff) {
        eff->apply(screenFramebuffer.getColorBufferID());
    }

    void setTransition(Transition* tr) {
        assert(tr);
        transition = tr;
        tr->reset();
    }

    void update(float dt) {
        if (transition) {
            transition->update(dt);
            if (transition->finished()) transition = nullptr;
        }
    }
};