#pragma once
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Node.hpp"

#define MODEL_NOT_LOADED "$notloaded$"

class Model {
private:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    Node* rootNode;
public:
    Model(const std::string& path);
    Model(const std::vector<Mesh*>& meshes, 
        const std::vector<Texture*>& textures, Node* rootNode);
    ~Model();

    const std::string path;
};