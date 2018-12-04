#pragma once
#include <vector>
#include <string>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Node.hpp"
#include "Material.hpp"
#include <Shaders/PhongShader.hpp>

#define MODEL_NOT_LOADED "$notloaded$"

class Model {
private:
    Model(Model const&) =delete;
    Model& operator=(Model const&) =delete;
protected:
    std::vector<Material*> materials;
    std::vector<Texture*> textures;
    std::vector<Mesh*> meshes;
    Node* rootNode;
public:
    Model(const std::string& path);
    ~Model();

    void load();
    virtual void update(double dT);
    virtual void draw(PhongShader& shader);

    const std::string path;
    std::vector<Material*> getMaterials();
    std::vector<Texture*> getTextures();
    std::vector<Mesh*> getMeshes();
    Node* getRootNode();
};