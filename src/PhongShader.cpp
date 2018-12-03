#include "PhongShader.hpp"

PhongShader::PhongShader() {
    attach("assets/shaders/phong.vs.glsl", GL_VERTEX_SHADER);
    attach("assets/shaders/phong.fs.glsl", GL_FRAGMENT_SHADER);
    link();
}