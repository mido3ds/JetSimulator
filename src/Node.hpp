#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>

class Node {
private:
    static Node* build(const aiScene* scene, const aiNode* node, Node* parent);
public:
    static Node* build(const aiScene* scene, const aiNode* node);

    Node(const std::string& name, const glm::mat4& transform, Node *parent, 
        const std::vector<Node*>& children, const std::vector<Mesh*>& meshes);
    ~Node();

    glm::mat4 getTransform();
    glm::mat4 getTotalTransform();
    Node* getNodeByName(const std::string name);
    void draw();

    const std::string name;
    const Node *parent;
    const std::vector<Node*> children;
    const std::vector<Mesh*> meshes;
    glm::mat4 transform;
};