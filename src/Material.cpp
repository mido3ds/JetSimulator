#include "Material.hpp"
#include <cassert>
using std::string;

#define DEFUALT_DIFF_PATH "assets/defaults/diffuse.bmp"
#define DEFAULT_SPC_PATH "assets/defaults/specular.bmp"
#define DEFAULT_SHININESS 16

Material* Material::build(const aiMaterial* mat, std::map<std::string,Texture*>& textMap, const string& path) {
    float shininess;
    aiString ai_diffPath, ai_specPath;
    string diffPath(path + "/"), specPath(path + "/");
    aiReturn ret;

    ret = mat->GetTexture(aiTextureType_DIFFUSE, 0, &ai_diffPath);
    if (ret != aiReturn_SUCCESS)
        diffPath = DEFUALT_DIFF_PATH;
    else 
        diffPath += ai_diffPath.C_Str();

    ret = mat->GetTexture(aiTextureType_SPECULAR, 0, &ai_specPath);
    if (ret != aiReturn_SUCCESS)
        specPath = DEFAULT_SPC_PATH;
    else
        specPath += ai_specPath.C_Str();

    ret = aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH, (ai_real*)&shininess);
    if (ret != aiReturn_SUCCESS) 
        shininess = DEFAULT_SHININESS;

    Texture* diff;
    auto diffItr = textMap.find(diffPath);
    if (diffItr != textMap.end()) {
        diff = diffItr->second;
    } else {
        diff = new Texture(diffPath, Texture::Diffuse);
        textMap[diffPath] = diff;
    }

    Texture* spec;
    auto spcItr = textMap.find(specPath);
    if (spcItr != textMap.end()) {
        spec = spcItr->second;
    } else {
        spec = new Texture(specPath, Texture::Specular);
        textMap[specPath] = spec;
    }

    return new Material(diff, spec, shininess);
}

Material::Material(const Texture* diffuse, 
        const Texture* specular, const float shininess) 
        :diffuse(diffuse), specular(specular), shininess(shininess) {
    assert(shininess >= 0 && diffuse && specular);
}

Material::Material(const Texture* diffuse, const Texture* specular) 
    :Material(diffuse, specular, DEFAULT_SHININESS) {}
