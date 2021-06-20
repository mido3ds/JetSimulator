#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "Mesh.hpp"
#include <Shaders/PhongShader.hpp>
#include <memory>

using namespace std;

class Node {
private:
    optional<reference_wrapper<Node>> parent;
    vector<unique_ptr<Node>> children;
    vector<shared_ptr<Mesh>> meshes;
public:
    static unique_ptr<Node> build(const aiScene& scene, 
        const aiNode& node, 
        const vector<shared_ptr<Mesh>>& allMeshes, 
        optional<reference_wrapper<Node>> parent={});

    Node(const string& name, const glm::mat4& transform, optional<reference_wrapper<Node>> parent, 
        vector<unique_ptr<Node>> children, const vector<shared_ptr<Mesh>>& meshes);

    glm::mat4 getTotalTransform() const;
    void render(PhongShader& shader);

    const string name;
    glm::mat4 transform;

    unique_ptr<Node> disattachFromParent();
    unique_ptr<Node> disattachChild(Node& ch);
    bool attached();

    vector<reference_wrapper<Node>> getChildren() const;
};
