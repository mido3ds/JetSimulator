#pragma once
#include <Textures/Texture2D.hpp>
#include <assimp/material.h>
#include <string>
#include <map>

class Material {
private:
public:
    static Material* build(const aiMaterial* mat, std::map<std::string,Texture2D*>& textMap, const std::string& path);
    Material(const Texture2D* diffuse, const Texture2D* specular, const float shininess);
    Material(const Texture2D* diffuse, const Texture2D* specular);

    const Texture2D* diffuse;
    const Texture2D* specular;
    const float shininess;
};