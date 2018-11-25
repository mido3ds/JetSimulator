#pragma once
#include "Texture.hpp"
#include <assimp/scene.h>
#include <string>
#include <map>

#define DEFAULT_SHININESS 25 // TODO: what is best?

class Material {
private:
public:
    static Material* build(const aiMaterial* mat, std::map<std::string,Texture*>& textMap);
    Material(const Texture* diffuse, const Texture* specular, const float shininess);
    Material(const Texture* diffuse, const Texture* specular);

    void bind() const;

    const Texture* diffuse;
    const Texture* specular;
    const float shininess;
};