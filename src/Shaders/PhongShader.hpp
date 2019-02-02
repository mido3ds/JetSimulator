#pragma once
#include "Shader.hpp"
#include <Scene/Material.hpp>
#include <Scene/Light.hpp>
#include <glm/glm.hpp>

class PhongShader: public Shader {
private:
    GLuint uProjView, uModel, uViewPos, uDirLight_dir, uDirLight_ambient, uDirLight_diffuse,
            uDirLight_specular, uNumPointLights, uNumSpotLights, uMaterial_shininess, uUseFog;
public:
    PhongShader();
    
	void switchFog(bool state);
    void setMaterial(const Material& material);
    void setDirLight(const DirLight& light);
    void setPointLight(const PointLight& light, int lightNumber);
    void setSpotight(const SpotLight& light, int lightNumber);
    void setViewPos(const glm::vec3& pos);
    
    void setProjView(const glm::mat4& pv);
    void setModel(const glm::mat4& model);
};