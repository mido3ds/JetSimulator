#pragma once
#include <Textures/Texture2D.hpp>
#include <assimp/material.h>
#include <string>
#include <map>
#include <memory>
#include <cassert>

using namespace std;

#define DEFAULT_SHININESS 16

class Material {
public:
    static shared_ptr<Material> build(aiMaterial const* mat, 
        map<string,shared_ptr<Texture2D>>& textMap, 
        const string& path);

    inline Material(shared_ptr<const Texture2D> diffuse, 
        shared_ptr<const Texture2D> specular, 
        const float shininess = DEFAULT_SHININESS)
        :diffuse(diffuse), specular(specular), shininess(shininess) {
        assert(shininess >= 0 && diffuse && specular);
    }

    shared_ptr<const Texture2D> diffuse;
    shared_ptr<const Texture2D> specular;
    const float shininess;
};
