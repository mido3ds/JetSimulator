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
        aiProcess_SortByPType || aiProcess_RemoveComponent ||
        aiProcess_GenSmoothNormals || aiProcess_RemoveRedundantMaterials || 
        aiProcess_OptimizeMeshes || aiProcess_FixInfacingNormals ||
        aiProcess_FindInvalidData || aiProcess_GenUVCoords);
    assert(scene && importer.GetErrorString());

    // materials
    std::map<std::string, Texture*> textMap;
    materials.reserve(scene->mNumMaterials);
    for (int i = 0; i < materials.size(); i++) {
        materials[i] = Material::build(scene->mMaterials[i], textMap);
    }

    // textures
    int i = 0;
    textures.reserve(textMap.size());
    for (const auto& text: textMap) textures[i++] = text.second;

    // meshes
    meshes.reserve(scene->mNumMeshes);
    for (int i = 0; i < meshes.size(); i++) {
        aiMesh* mesh = scene->mMeshes[i];
        meshes[i] = Mesh::build(mesh, materials[mesh->mMaterialIndex]);
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
