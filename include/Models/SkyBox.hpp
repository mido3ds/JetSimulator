#pragma once
#include <Textures/CubeMapTexture.hpp>
#include <Shaders/Shader.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

using namespace std;

class SkyBox {
private:
    CubeMapTexture cubemap;
    unique_ptr<Shader> shader;
    GLuint vao, uProjView, uUseFog ,uUseGrayscale , uUseSepia, uUseVignette;
	bool useFog , useGrayscale , useSepia, useVignette;
public:
    SkyBox();

    void load();
	void switchFog(bool state);
	void switchVignette(bool state);
	void switchGrayscale(bool state);
	void switchSepia(bool state);
    void render(glm::mat4 proj, glm::mat4 view, glm::vec2 vec);
};