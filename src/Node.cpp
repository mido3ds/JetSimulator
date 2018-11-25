#include "Node.hpp"

Node::Node(const std::string& name, const glm::mat4& transform, Node *parent, 
    const std::vector<Node*>& children, const std::vector<Mesh*>& meshes)
    :name(name), transform(transform), parent(parent), children(children), meshes(meshes) {}

Node::~Node() {
    for (Node* ch: children) delete ch;
}

glm::mat4 Node::getTotalTransform() {
    if (!parent) return transform;
    return parent->getTotalTransform() * transform;
}

Node* Node::getNodeByName(const std::string& name) {
    if (this.name == name) return this;
    for (Node* node: children) {
        if (node->getNodeByName(name)) return node
    }
    return nullptr;
}

void Node::draw() {
    //TODO: bind material
    for (Mesh* mesh: meshes) mesh->draw();
}
