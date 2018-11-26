#pragma once
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Node.hpp"
#include "Material.hpp"

#define MODEL_NOT_LOADED "$notloaded$"

class Model {
private:
    std::vector<Material*> materials;
    std::vector<Texture*> textures;
    std::vector<Mesh*> meshes;
    Node* rootNode;
public:
    Model(const std::string& path);
    Model(const std::vector<Mesh*>& meshes, 
        const std::vector<Texture*>& textures, Node* rootNode);
    ~Model();

    const std::string path;
};