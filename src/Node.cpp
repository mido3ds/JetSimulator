#include "Node.hpp"
#include <glm/gtc/type_ptr.hpp>

Node* Node::build(const aiScene* scene, const aiNode* node, Node* parent) {
    glm::mat4 transform = glm::make_mat4(&node->mTransformation.a1); //TODO: see it working
    std::vector<Node*> children = std::vector<Node*>(node->mNumChildren);
    std::vector<Mesh*> meshes = std::vector<Mesh*>(node->mNumMeshes);
    
    Node* builtNode = new Node(node->mName.C_Str(), transform, parent, children, meshes);

    // TODO: provide meshes

    for (int i = 0; i < children.size(); i++) {
        builtNode->children[i] = build(scene, node->mChildren[i], builtNode);
    }

    return builtNode;
}

Node* Node::build(const aiScene* scene, const aiNode* node) {
    return build(scene, node, nullptr);
}

Node::Node(const std::string& name, const glm::mat4& transform, Node *parent, 
    const std::vector<Node*>& children, const std::vector<Mesh*>& meshes)
    :name(name), transform(transform), parent(parent), children(children), meshes(meshes) {}

Node::~Node() {
    for (Node* ch: children) delete ch;
}

glm::mat4 Node::getTotalTransform() const {
    if (!parent) return transform;
    return parent->getTotalTransform() * transform;
}

Node* Node::getNodeByName(const std::string& name) {
    if (this->name == name) return this;
    for (Node* node: children) {
        if (node->getNodeByName(name)) return node;
    }
    return nullptr;
}

void Node::draw() {
    //TODO: bind material
    for (Mesh* mesh: meshes) mesh->draw();
}
