#pragma once
#include <vector>
#include <string>
#include "Mesh.hpp"
#include <Textures/Texture2D.hpp>
#include "Node.hpp"
#include "Material.hpp"
#include <Shaders/PhongShader.hpp>
#include <glm/glm.hpp>

class Model {
private:
    Model(Model const&) =delete;
    Model& operator=(Model const&) =delete;
protected:
    std::vector<Material*> materials;
    std::vector<Texture2D*> textures;
    std::vector<Mesh*> meshes;
    Node* rootNode;
public:
    Model(const std::string& path);
    ~Model();

    void load();
    virtual void update(float dT);
    virtual void draw(PhongShader& shader);

    const std::string path;
    std::vector<Material*> getMaterials();
    std::vector<Texture2D*> getTextures();
    std::vector<Mesh*> getMeshes();
    Node* getRootNode();

    glm::vec3 pos, front, up, right;
    float pitch=0, yaw=0, roll=0;
};