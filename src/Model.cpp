#include "Model.hpp"
#include "Mesh.hpp"
#include "Node.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <map>

Model::Model(const std::string& path) :path(path) {
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
}

Model::Model(const std::vector<Mesh*>& meshes, 
        const std::vector<Texture*>& textures, Node* rootNode) 
    :path(MODEL_NOT_LOADED), meshes(meshes), textures(textures), rootNode(rootNode) {}

Model::~Model() {
    for (Material* mat: materials) delete mat;
    for (Texture* tex: textures) delete tex;
    for (Mesh* mesh: meshes) delete mesh;
    delete rootNode;
}
