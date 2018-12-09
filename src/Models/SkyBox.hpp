#pragma once
#include <Textures/CubeMapTexture.hpp>
#include <Shaders/Shader.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

class SkyBox {
private:
    CubeMapTexture cubemap;
    Shader* shader;
    GLuint vao, uProjView;
public:
    SkyBox();
    ~SkyBox();

    void load();
    void draw(glm::mat4 proj, glm::mat4 view);
};