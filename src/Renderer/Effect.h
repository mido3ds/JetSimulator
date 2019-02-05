#pragma once
#include <sstream>
#include <Shaders/Shader.hpp>
#include <glm/ext.hpp>
#include "Framebuffer.h"

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

class VignetteEffect : public Effect {
public:
    VignetteEffect(float innerRadius, float outerRadius, 
        float intensity, int width, int height) :Effect(STR(
        uniform float uOuterRadius;
        uniform float uInnerRadius;
        uniform float uIntensity;
        uniform vec2 uResolution;

        vec4 apply(sampler2D tex, vec2 coords) {
            vec3 color = vec3(texture(tex, coords));

            float len = length(gl_FragCoord.xy/uResolution - 0.5);
            float vignette = smoothstep(uOuterRadius, uInnerRadius, len); 

            return vec4(mix(color, color * vignette, uIntensity), 1);
        }
    )) {
        setInnerRadius(innerRadius);
        setOuterRadius(outerRadius);
        setIntensity(intensity);
        setResolution(width, height);
    }

    void setInnerRadius(float innerRadius) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uInnerRadius"), innerRadius);
    }

    void setOuterRadius(float outerRadius) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uOuterRadius"), outerRadius);
    }

    void setIntensity(float intensity) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uIntensity"), intensity);
    }

    void setResolution(int width, int height) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uResolution"), glm::vec2(width, height));
    }
};

class KernelEffect : public Effect {
public:
    KernelEffect(glm::mat3 kernel, float offset=1.0f/300.0f) :Effect(STR(
        uniform float uOffset;  
        uniform mat3 uKernel;

        vec4 apply(sampler2D tex, vec2 coords) {
            vec2 offsets[9] = vec2[](
                vec2(-uOffset,  uOffset), // top-left
                vec2( 0.0f,    uOffset), // top-center
                vec2( uOffset,  uOffset), // top-right
                vec2(-uOffset,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( uOffset,  0.0f),   // center-right
                vec2(-uOffset, -uOffset), // bottom-left
                vec2( 0.0f,   -uOffset), // bottom-center
                vec2( uOffset, -uOffset)  // bottom-right    
            );

            vec3 col = vec3(0.0);
            for(int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    col += vec3(texture(tex, coords.st + offsets[i+j])) * uKernel[i][j];
            
            return vec4(col, 1.0);
        }
    )) {
        setOffset(offset);
        setKernel(kernel);
    }

    void setOffset(const float& offset) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uOffset"), offset);
    }

    void setKernel(const glm::mat3& kernel) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uKernel"), kernel);
    }
};

class BlurEffect : public KernelEffect {
public: BlurEffect() :KernelEffect(glm::mat3(1, 2, 1, 2, 4, 2, 1, 2, 1)/16) {} 
};

class BottomSobelEffect : public KernelEffect {
public: BottomSobelEffect() :KernelEffect(glm::mat3(-1, -2, -1, 0, 0, 0, 1, 2, 1)) {}
};

class EmbossEffect : public KernelEffect {
public: EmbossEffect() :KernelEffect(glm::mat3(-2, -1, 0, -1, 1, 1, 0, 1, 2)) {}
};

class LeftSobelEffect : public KernelEffect {
public: LeftSobelEffect() :KernelEffect(glm::mat3(1, 0, -1, 2, 0, -2, 1, 0, -1)) {}
};

class OutlineEffect : public KernelEffect {
public: OutlineEffect() :KernelEffect(glm::mat3(-1, -1, -1, -1, 8, -1, -1, -1, -1)) {}
};

class RightSobelEffect : public KernelEffect {
public: RightSobelEffect() :KernelEffect(glm::mat3(-1, 0, 1, -2, 0, -2, -1, 0, 1)) {}
};

class SharpenEffect : public KernelEffect {
public: SharpenEffect() :KernelEffect(glm::mat3(0, -1, 0, -1, 5, -1, 0, -1, 0)) {}
};

class TopSobelEffect : public KernelEffect {
public: TopSobelEffect() :KernelEffect(glm::mat3(1, 2, 1, 0, 0, 0, -1, -2, -1)) {}
};

class Transition : public Effect {
protected:
    float totalTime;
    float elapsedTime;
    GLuint uTotalTimeLoc, uElapsedTimeLoc;

public:
    Transition(const std::string& func, float totalTime) 
        :Effect("uniform float uTotalTime, uElapsedTime;\n" + func) {
        shader.use();
        uTotalTimeLoc = shader.getUniformLocation("uTotalTime");
        uElapsedTimeLoc = shader.getUniformLocation("uElapsedTime");
        reset();
        setTotalTime(totalTime);
    }

    void update(float dt) {
        if (!finished()) elapsedTime += dt;
        shader.use();
        shader.setUniform(uElapsedTimeLoc, elapsedTime);
    }

    bool finished() {return elapsedTime >= totalTime;}

    void reset() {
        elapsedTime = 0;
        shader.use();
        shader.setUniform(uElapsedTimeLoc, 0);
    }

    void setTotalTime(float totalTime) {
        assert(totalTime > 0);
        this->totalTime = totalTime;
        shader.use();
        shader.setUniform(uTotalTimeLoc, totalTime);
    }
};

class FadeOutTransition : public Transition {
public:
    FadeOutTransition(float totalTime, const glm::vec3& color=glm::vec3(0)) 
        :Transition(STR(
            uniform vec3 uColor;
            vec4 apply(sampler2D tex, vec2 coords) {
                return vec4(mix(vec3(texture(tex, coords)), uColor, uElapsedTime/uTotalTime), 1);
            }
        ), totalTime) {
        setColor(color);
    }

    void setColor(const glm::vec3& color) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uColor"), color);
    }
};

class FadeInTransition : public Transition {
public:
    FadeInTransition(float totalTime, const glm::vec3& color=glm::vec3(0)) 
        :Transition(STR(
            uniform vec3 uColor;
            vec4 apply(sampler2D tex, vec2 coords) {
                return vec4(mix(uColor, vec3(texture(tex, coords)), uElapsedTime/uTotalTime), 1);
            }
        ), totalTime) {
        setColor(color);
    }

    void setColor(const glm::vec3& color) {
        shader.use();
        shader.setUniform(shader.getUniformLocation("uColor"), color);
    }
};