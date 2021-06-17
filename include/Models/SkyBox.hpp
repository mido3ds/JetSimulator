#pragma once
#include <Textures/CubeMapTexture.hpp>
#include <Shaders/Shader.hpp>
#include <Shaders/Effects.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

using namespace std;

class SkyBox {
private:
    CubeMapTexture cubemap;
    unique_ptr<Shader> shader;
    GLuint vao, uProjView, uUseFog ,uUseGrayscale , uUseSepia, uUseVignette;
	Effects effects;
public:
    SkyBox();

    void load();
    void setEffects(Effects effects);
    void render(glm::mat4 proj, glm::mat4 view, glm::vec2 vec);
};
