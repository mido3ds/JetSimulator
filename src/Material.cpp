#include "Material.hpp"
#include <cassert>
using std::string;

Material* Material::build(const aiMaterial* mat, std::map<std::string,Texture*>& textMap) {
    float shininess;
    aiString diffPath, specPath;
    aiReturn ret;

    ret = mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath);
    assert(ret == aiReturn_SUCCESS);//TODO: load default diffuse map image
    ret = mat->GetTexture(aiTextureType_SPECULAR, 0, &specPath);
    assert(ret == aiReturn_SUCCESS);
    ret = aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH, (ai_real*)&shininess);
    if (ret != aiReturn_SUCCESS) shininess = DEFAULT_SHININESS;

    Texture* diff;
    auto diffItr = textMap.find(diffPath.C_Str());
    if (diffItr != textMap.end()) {
        diff = diffItr->second;
    } else {
        diff = new Texture(new string(diffPath.C_Str()), Texture::Diffuse);
        textMap[diffPath.C_Str()] = diff;
    }

    Texture* spec;
    auto spcItr = textMap.find(specPath.C_Str());
    if (spcItr != textMap.end()) {
        spec = spcItr->second;
    } else {
        spec = new Texture(new string(specPath.C_Str()), Texture::Specular);
        textMap[diffPath.C_Str()] = spec;
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


void Material::bind() const {
    diffuse->bind();
    specular->bind();
    //TODO: send shinines to shader
}
