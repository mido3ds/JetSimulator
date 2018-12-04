#pragma once
#include "Shader.hpp"
#include "Material.hpp"
#include "Light.hpp"

class PhongShader: public Shader {
private:
    GLuint uProjView, uModel, uViewPos, uDirLight_dir, uDirLight_ambient, uDirLight_diffuse,
            uDirLight_specular, uNumPointLights, uNumSpotLights, uMaterial_shininess;
public:
    PhongShader();
    
    void setMaterial(Material const* material);
    void setDirLight(DirLight const* light);
    void setPointLight(PointLight const* light, int lightNumber);
    void setSpotight(SpotLight const* light, int lightNumber);
};