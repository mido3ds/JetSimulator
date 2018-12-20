#include "PhongShader.hpp"
using std::string;

PhongShader::PhongShader() {
    attach("assets/shaders/phong.vs.glsl", GL_VERTEX_SHADER);
    attach("assets/shaders/phong.fs.glsl", GL_FRAGMENT_SHADER);
    link();

    use();
    uProjView = getUniformLocation("uProjView");
    uModel = getUniformLocation("uModel");
    uViewPos = getUniformLocation("uViewPos");

    uNumPointLights = getUniformLocation("uNumPointLights");
    uNumSpotLights = getUniformLocation("uNumSpotLights");

    uDirLight_dir = getUniformLocation("uDirLight.dir"); 
    uDirLight_ambient = getUniformLocation("uDirLight.ambient"); 
    uDirLight_diffuse = getUniformLocation("uDirLight.diffuse");
    uDirLight_specular = getUniformLocation("uDirLight.specular");

	uUseFog= getUniformLocation("uUseFog");
	uUseGrayscale = getUniformLocation("uUseGrayScale");
	uUseSepia = getUniformLocation("uUseSepia");
	uUseVignette = getUniformLocation("uUseVignette");

    setUniform(getUniformLocation("uMaterial.diffuse"), Texture2D::Usage::Diffuse); 
    setUniform(getUniformLocation("uMaterial.specular"), Texture2D::Usage::Specular);
    uMaterial_shininess = getUniformLocation("uMaterial.shininess");
}

void PhongShader::setMaterial(const Material& material) {
    material.diffuse->bind();
    material.specular->bind();
    setUniform(uMaterial_shininess, material.shininess);
}
void PhongShader::switchFog(bool state)
{
	setUniform(uUseFog, state);
}
void PhongShader::switchGrayscale(bool state)
{
	setUniform(uUseGrayscale, state);
}
void PhongShader::switchSepia(bool state)
{
	setUniform(uUseSepia, state);
}
void PhongShader::switchVignette(bool state)
{
	setUniform(uUseVignette, state);
}
void PhongShader::setDirLight(const DirLight& light) {
    setUniform(uDirLight_dir, light.dir);
    setUniform(uDirLight_ambient, light.ambient);
    setUniform(uDirLight_diffuse, light.diffuse);
    setUniform(uDirLight_specular, light.specular);
}

void PhongShader::setPointLight(const PointLight& light, int lightNumber) {
    string structStr = "uPointLight[";
    structStr += lightNumber;
    structStr += "].";

    setUniform(getUniformLocation(structStr+"pos"), light.pos);
    setUniform(getUniformLocation(structStr+"ambient"), light.ambient);
    setUniform(getUniformLocation(structStr+"diffuse"), light.diffuse);
    setUniform(getUniformLocation(structStr+"specular"), light.specular);
    setUniform(getUniformLocation(structStr+"quadratic"), light.quadratic);
    setUniform(getUniformLocation(structStr+"linear"), light.linear);
    setUniform(getUniformLocation(structStr+"constant"), light.constant);
}

void PhongShader::setSpotight(const SpotLight& light, int lightNumber) {
    string structStr = "uSpotLight[";
    structStr += lightNumber;
    structStr += "].";

    setUniform(getUniformLocation(structStr+"pos"), light.pos);
    setUniform(getUniformLocation(structStr+"dir"), light.dir);
    setUniform(getUniformLocation(structStr+"ambient"), light.ambient);
    setUniform(getUniformLocation(structStr+"diffuse"), light.diffuse);
    setUniform(getUniformLocation(structStr+"specular"), light.specular);
    setUniform(getUniformLocation(structStr+"quadratic"), light.quadratic);
    setUniform(getUniformLocation(structStr+"linear"), light.linear);
    setUniform(getUniformLocation(structStr+"constant"), light.constant);
    setUniform(getUniformLocation(structStr+"innerCone"), light.innerCone);
    setUniform(getUniformLocation(structStr+"outerCone"), light.outerCone);
}

void PhongShader::setProjView(const glm::mat4& pv) {
    setUniform(uProjView, pv);
}

void PhongShader::setModel(const glm::mat4& model) {
    setUniform(uModel, model);
}

void PhongShader::setViewPos(const glm::vec3& pos) {
    setUniform(uViewPos, pos);
}