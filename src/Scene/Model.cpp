#include "Scene/Model.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Node.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <map>

Model::Model(const std::string& path) :path(path), pos(0, 0, 0), right(1, 0, 0), front(0, 1, 0), up(0, 0, 1) {}

Model::~Model() {
    for (Material* mat: materials) delete mat;
    for (Texture2D* tex: textures) delete tex;
    for (Mesh* mesh: meshes) delete mesh;
    delete rootNode;
}

void Model::load() {
    // import
    const aiScene* scene = aiImportFile(path.c_str(), 
        aiProcessPreset_TargetRealtime_Fast);
    assert(scene && aiGetErrorString());

    // materials
    std::map<std::string, Texture2D*> textMap;
    for (int i = 0; i < scene->mNumMaterials; i++) {
        materials.push_back(Material::build(scene->mMaterials[i], textMap, path));
    }

    // textures
    for (const auto& text: textMap) { 
        textures.push_back(text.second); 
        text.second->load();
        text.second->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        text.second->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        text.second->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        text.second->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // meshes
    for (int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        meshes.push_back(Mesh::build(mesh, materials[mesh->mMaterialIndex]));
        meshes[i]->load();
    }

    // nodes
    rootNode = Node::build(scene, scene->mRootNode, meshes);
    rootNode->transform = glm::mat4();

    aiReleaseImport(scene);
}

void Model::update(float dT) {}

void Model::draw(PhongShader& shader) {
    rootNode->draw(shader);
}

std::vector<Material*> Model::getMaterials() {
    return materials;
}

std::vector<Texture2D*> Model::getTextures() {
    return textures;
}

std::vector<Mesh*> Model::getMeshes() {
    return meshes;
}

Node* Model::getRootNode() {
    return rootNode;
}
