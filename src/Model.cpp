#include "Model.hpp"
#include "Mesh.hpp"
#include "Node.hpp"

Model::Model(const std::string& path) 
    :path(path) {
    
}

Model::Model(const std::vector<Mesh*>& meshes, 
        const std::vector<Texture*>& textures, Node* rootNode) 
    :path(MODEL_NOT_LOADED), meshes(meshes), textures(textures), rootNode(rootNode) {}

Model::~Model() {
    for (Texture* tex: textures) delete tex;
    for (Mesh* mesh: meshes) delete mesh;
    delete rootNode;
}
