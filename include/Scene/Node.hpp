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
    weak_ptr<Node> parent;
    vector<shared_ptr<Node>> children;
    vector<shared_ptr<Mesh>> meshes;
public:
    static shared_ptr<Node> build(aiScene const* scene, 
        aiNode const* node, 
        const vector<shared_ptr<Mesh>>& allMeshes, 
        weak_ptr<Node> parent=weak_ptr<Node>());

    Node(const string& name, const glm::mat4& transform, weak_ptr<Node> parent, 
        const vector<shared_ptr<Node>>& children, const vector<shared_ptr<Mesh>>& meshes);

    glm::mat4 getTotalTransform() const;
    void render(unique_ptr<PhongShader>& shader);

    const string name;
    glm::mat4 transform;

    void disattachFromParent();
    void disattachChild(Node* ch);
    bool attached();

    inline vector<shared_ptr<Node>> getChildren() const {
        return children;
    }
};
