#pragma once
#include <Textures/CubeMapTexture.hpp>
#include <Shaders/Shader.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

class SkyBox {
private:
    CubeMapTexture cubemap;
    Shader* shader;
    GLuint vao, uProjView, uUseFog ,uUseGrayscale , uUseSepia, uUseVignette;
	bool useFog , useGrayscale , useSepia, useVignette;
public:
    SkyBox();
    ~SkyBox();

    void load();
	void switchFog(bool state);
	void switchVignette(bool state);
	void switchGrayscale(bool state);
	void switchSepia(bool state);
    void draw(glm::mat4 proj, glm::mat4 view, glm::vec2 vec);
};