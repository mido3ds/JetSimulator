#include "Model.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <map>

Model::Model(const std::string& path) :path(path), front(0, 0, 1), up(0, 1, 0) {}

Model::~Model() {
    for (Material* mat: materials) delete mat;
    for (Texture* tex: textures) delete tex;
    for (Mesh* mesh: meshes) delete mesh;
    delete rootNode;
}

void Model::load() {
    // import
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_JoinIdenticalVertices || aiProcess_Triangulate || 
        aiProcess_RemoveComponent || aiProcess_GenSmoothNormals || 
        aiProcess_OptimizeMeshes ||  aiProcess_GenUVCoords);
    assert(scene && importer.GetErrorString());

    // materials
    std::map<std::string, Texture*> textMap;
    for (int i = 0; i < scene->mNumMaterials; i++) {
        materials.push_back(Material::build(scene->mMaterials[i], textMap, path));
    }

    // textures
    for (const auto& text: textMap) { 
        textures.push_back(text.second); 
    }

    // meshes
    for (int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        meshes.push_back(Mesh::build(mesh, materials[mesh->mMaterialIndex]));
    }

    // nodes
    rootNode = Node::build(scene, scene->mRootNode, meshes);

    pos = glm::vec3(rootNode->transform[3]);
}

void Model::update(double dT) {}

void Model::draw(PhongShader& shader) {
    rootNode->draw(shader);
}

std::vector<Material*> Model::getMaterials() {
    return materials;
}

std::vector<Texture*> Model::getTextures() {
    return textures;
}

std::vector<Mesh*> Model::getMeshes() {
    return meshes;
}

Node* Model::getRootNode() {
    return rootNode;
}
