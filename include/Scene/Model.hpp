#pragma once
#include <vector>
#include <string>
#include "Mesh.hpp"
#include <Textures/Texture2D.hpp>
#include "Node.hpp"
#include "Material.hpp"
#include <Shaders/PhongShader.hpp>
#include <glm/glm.hpp>
#include <memory>

using namespace std;

class Model {
private:
    Model(Model const&) =delete;
    Model& operator=(Model const&) =delete;
protected:
    vector<shared_ptr<Material>> materials;
    vector<shared_ptr<Texture2D>> textures;
    vector<shared_ptr<Mesh>> meshes;
    shared_ptr<Node> rootNode;
public:
    Model(const string& path);

    virtual void load();
    virtual void update(float dT);

    virtual void render(unique_ptr<PhongShader>& shader);

    const string path;

    glm::vec3 pos, front, up, right;
    float pitch=0, yaw=0, roll=0;
};