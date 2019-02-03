#pragma once
#include <glad/glad.h>

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