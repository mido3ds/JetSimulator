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

    Model(Model const&) =delete;
    Model& operator=(Model const&) =delete;
public:
    Model(const std::string& path);
    ~Model();

    void load();
    void draw();

    const std::string path;
    std::vector<Material*> getMaterials();
    std::vector<Texture*> getTextures();
    std::vector<Mesh*> getMeshes();
    Node* getRootNode();
};