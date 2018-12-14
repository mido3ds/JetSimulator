#include "SkyBox.hpp" 
#include <glm/ext.hpp>

SkyBox::SkyBox() 
    :cubemap(
        "assets/skyboxes/ame_ash/",
        "right.png",
        "left.png",
        "top.png",
        "bottom.png",
        "front.png",
        "back.png"
    ) {
    shader = new Shader();
	//useFog = true;
	//uUseFog = getUniformLocation("uUseFog");
}

SkyBox::~SkyBox() {
    delete shader;
}

/*void SkyBox::switchFog(bool state)
{
	setUniform(uUseFog, state);
}
*/

GLfloat positions[] = {      
    -1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f, 
    -1.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f, 
    -1.0f, 1.0f, -1.0f, 
    -1.0f, 1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    1.0f, -1.0f, -1.0f, 
    1.0f, -1.0f, 1.0f, 
    -1.0f, 1.0f, 1.0f, 
    -1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, 1.0f, 
    -1.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f, 
    1.0f, -1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    1.0f, 1.0f, -1.0f, 
    -1.0f, 1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, 1.0f, 
    -1.0f, 1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    1.0f, -1.0f, 1.0f, 
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f, 1.0f
};

#include <iostream>
#include <glm/gtx/io.hpp>
using namespace std;

void SkyBox::load() {
    // shader
    shader->attach("assets/shaders/skybox.vs.glsl", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/skybox.fs.glsl", GL_FRAGMENT_SHADER);
    shader->link();
    shader->use();
    uProjView = shader->getUniformLocation("uProjView");
    shader->setUniform(shader->getUniformLocation("uCubeMap"), 0);

    // data
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // textures
    cubemap.load();
}

void SkyBox::draw(glm::mat4 proj, glm::mat4 view) {
    glDepthFunc(GL_LEQUAL);

    shader->use();
    shader->setUniform(uProjView, proj * glm::mat4(glm::mat3(view)) * glm::translate(glm::vec3(0,0,-0.2f)) * glm::rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0)));

    glBindVertexArray(vao);
    cubemap.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}