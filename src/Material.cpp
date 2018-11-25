#include "Material.hpp"
#include <cassert>

Material* Material::build(const aiMaterial* mat, std::map<std::string,Texture*>& textMap) {
    using std::string;

    float shininess;
    aiString diffPath, specPath;
    aiReturn ret;

    ret = aiGetMaterialFloat(mat, AI_MATKEY_SHININESS_STRENGTH, (ai_real*)&shininess);
    if (ret != aiReturn_SUCCESS)
        shininess = DEFAULT_SHININESS;

    ret = mat->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath);
    assert(ret == aiReturn_SUCCESS);//TODO: load default diffuse map image
    ret = mat->GetTexture(aiTextureType_SPECULAR, 0, &specPath);
    assert(ret == aiReturn_SUCCESS);

    //TODO: use maps

    // TODO:optimize 
    return new Material(new Texture(new string(diffPath.C_Str()), Texture::Diffuse),
                     new Texture(new string(specPath.C_Str()), Texture::Specular),
                     shininess);
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
