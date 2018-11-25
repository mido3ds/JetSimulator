#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "Mesh.hpp"

class Node {
private:
    static Node* build(const aiScene* scene, const aiNode* node, Node* parent);
public:
    static Node* build(const aiScene* scene, const aiNode* node);

    Node(const std::string& name, const glm::mat4& transform, Node *parent, 
        const std::vector<Node*>& children, const std::vector<Mesh*>& meshes);
    ~Node();

    glm::mat4 getTotalTransform() const;
    Node* getNodeByName(const std::string& name);
    void draw();

    const std::string name;
    const Node *parent;
    std::vector<Node*> children;
    std::vector<Mesh*> meshes;
    glm::mat4 transform;
};