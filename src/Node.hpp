#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Node {
private:
public:
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