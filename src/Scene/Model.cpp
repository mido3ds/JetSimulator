#include <Scene/Model.hpp>
#include <Scene/Mesh.hpp>
#include <Scene/Node.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <map>

Model::Model(const string& path) :path(path), pos(0, 0, 0), right(1, 0, 0), front(0, 1, 0), up(0, 0, 1) {}

void Model::load() {
    // import
    const aiScene* scene = aiImportFile(path.c_str(), 
        aiProcessPreset_TargetRealtime_Fast);
    assert(scene && aiGetErrorString());

    // materials
    map<string, shared_ptr<Texture2D>> textMap;
    vector<shared_ptr<Material>> materials;
    for (int i = 0; i < scene->mNumMaterials; i++) {
        materials.push_back(Material::build(scene->mMaterials[i], textMap, path));
    }

    // textures
    for (const auto& text: textMap) { 
        text.second->load();
        text.second->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        text.second->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        text.second->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        text.second->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    // meshes
    vector<shared_ptr<Mesh>> meshes;
    for (int i = 0; i < scene->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[i];
        meshes.push_back(Mesh::build(mesh, materials[mesh->mMaterialIndex]));
        meshes[i]->load();
    }

    // nodes
    rootNode = Node::build(scene, scene->mRootNode, move(meshes));
    rootNode->transform = glm::mat4();

    aiReleaseImport(scene);
}

void Model::update(float dT) {}

void Model::render(unique_ptr<PhongShader>& shader) {
    rootNode->render(shader);
}
