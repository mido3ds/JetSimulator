#pragma once
#include "Texture.hpp"
#include <assimp/scene.h>
#include <string>
#include <map>

class Material {
private:
public:
    static Material* build(const aiMaterial* mat, std::map<std::string,Texture*>& textMap, const std::string& path);
    Material(const Texture* diffuse, const Texture* specular, const float shininess);
    Material(const Texture* diffuse, const Texture* specular);

    const Texture* diffuse;
    const Texture* specular;
    const float shininess;
};