#include <Scene/Node.hpp>
#include <glm/gtc/type_ptr.hpp>

shared_ptr<Node> Node::build(aiScene const* scene, aiNode const* node, const vector<shared_ptr<Mesh>>& allMeshes, weak_ptr<Node> parent) {
    glm::mat4 transform = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    vector<shared_ptr<Node>> children(node->mNumChildren);

    vector<shared_ptr<Mesh>> meshes(node->mNumMeshes);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i] = allMeshes[node->mMeshes[i]];
    }
    
    auto builtNode = make_shared<Node>(node->mName.C_Str(), transform, parent, children, meshes);
    auto w = weak_ptr<Node>(builtNode);

    for (int i = 0; i < children.size(); i++) {
        builtNode->children[i] = build(scene, node->mChildren[i], allMeshes, weak_ptr<Node>(builtNode));
    }

    return builtNode;
}

Node::Node(const string& name, const glm::mat4& transform, weak_ptr<Node> parent, 
        const vector<shared_ptr<Node>>& children, const vector<shared_ptr<Mesh>>& meshes)
    :name(name), transform(transform), parent(parent), children(children), meshes(meshes) {}

glm::mat4 Node::getTotalTransform() const {
    if (auto p = parent.lock()) {
        return p->getTotalTransform() * transform;
    } else {
        return transform;
    }
}

shared_ptr<Node> Node::getNodeByName(const string& name, shared_ptr<Node> thisptr) {
    if (this->name == name) return thisptr;
    for (auto& node: children) {
        if (node->getNodeByName(name, node)) return node;
    }
    return {nullptr};
}

void Node::render(unique_ptr<PhongShader>& shader) {
    shader->setModel(getTotalTransform());
    for (auto& mesh: meshes) mesh->render(shader);
    for (auto& child:children) child->render(shader);
}

bool Node::attached() {
    return !parent.expired();
}

shared_ptr<Node> Node::disattachFromParent() {
    auto p = parent.lock();
    return p->disattachChild(this);
}

shared_ptr<Node> Node::disattachChild(Node* ch) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i].get() == ch) {
            ch->transform = ch->getTotalTransform();
            auto chsh = children[i];
            chsh->parent.reset();
            children.erase(children.begin() + i);

            return chsh;
        }
    }
    assert(false && "must exist");
    return {};
}
