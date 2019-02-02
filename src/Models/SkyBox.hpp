#pragma once
#include <Textures/CubeMapTexture.hpp>
#include <Shaders/Shader.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

class SkyBox {
private:
    CubeMapTexture cubemap;
    Shader* shader;
    GLuint vao, uProjView, uUseFog;
	bool useFog;
public:
    SkyBox();
    ~SkyBox();

    void load();
	void switchFog(bool state);
    void draw(glm::mat4 proj, glm::mat4 view, glm::vec2 vec);
};