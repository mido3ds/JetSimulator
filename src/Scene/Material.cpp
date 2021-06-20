#include <Scene/Material.hpp>
#include <cstddef>

#define DEFUALT_DIFF_PATH "assets/defaults/diffuse.bmp"
#define DEFAULT_SPC_PATH "assets/defaults/specular.bmp"

shared_ptr<Material> Material::build(const aiMaterial& mat, map<string,shared_ptr<Texture2D>>& textMap, const string& path) {
    float shininess;
    aiString ai_diffPath, ai_specPath;
    string dir = path.substr(0, path.find_last_of("/\\")+1);
    string diffPath(dir), specPath(dir);
    aiReturn ret;

    ret = aiGetMaterialString(&mat, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &ai_diffPath);
    if (ret != aiReturn_SUCCESS) {
        diffPath = DEFUALT_DIFF_PATH;
    } else  {
        diffPath += ai_diffPath.C_Str();
    }

    ret = aiGetMaterialString(&mat, AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), &ai_specPath);
    if (ret != aiReturn_SUCCESS) {
        specPath = DEFAULT_SPC_PATH;
    } else {
        specPath += ai_specPath.C_Str();
    }

    ret = aiGetMaterialFloat(&mat, AI_MATKEY_SHININESS_STRENGTH, (ai_real*)&shininess);
    if (ret != aiReturn_SUCCESS) {
        shininess = DEFAULT_SHININESS;
    }

    shared_ptr<Texture2D> diff;
    auto diffItr = textMap.find(diffPath);
    if (diffItr != textMap.end()) {
        diff = diffItr->second;
    } else {
        diff = textMap[diffPath] = make_shared<Texture2D>(diffPath, Texture2D::Type::Diffuse);
    }

    shared_ptr<Texture2D> spec;
    auto spcItr = textMap.find(specPath);
    if (spcItr != textMap.end()) {
        spec = spcItr->second;
    } else {
        spec = textMap[specPath] = make_shared<Texture2D>(specPath, Texture2D::Type::Specular);
    }

    return make_shared<Material>(move(diff), move(spec), shininess);
}
