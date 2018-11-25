#pragma once
#include <vector>
#include <string>

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