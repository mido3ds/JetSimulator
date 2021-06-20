#include <Scene/Node.hpp>
#include <glm/gtc/type_ptr.hpp>

unique_ptr<Node> Node::build(const aiScene& scene, const aiNode& node, const vector<shared_ptr<Mesh>>& allMeshes, optional<reference_wrapper<Node>> parent) {
    glm::mat4 transform = glm::transpose(glm::make_mat4(&node.mTransformation.a1));
    vector<unique_ptr<Node>> children(node.mNumChildren);

    vector<shared_ptr<Mesh>> meshes(node.mNumMeshes);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i] = allMeshes[node.mMeshes[i]];
    }
    
    auto builtNode = make_unique<Node>(node.mName.C_Str(), transform, parent, move(children), meshes);

    for (int i = 0; i < children.size(); i++) {
        builtNode->children[i] = build(scene, *node.mChildren[i], allMeshes, {*builtNode.get()});
    }

    return builtNode;
}

Node::Node(const string& name, const glm::mat4& transform, optional<reference_wrapper<Node>> parent, 
        vector<unique_ptr<Node>> children, const vector<shared_ptr<Mesh>>& meshes)
    :name(name), transform(transform), parent(parent), children(move(children)), meshes(meshes) {}

glm::mat4 Node::getTotalTransform() const {
    if (parent.has_value()) {
        return parent.value().get().getTotalTransform() * transform;
    }

    return transform;
}

void Node::render(PhongShader& shader) {
    shader.setModel(getTotalTransform());

    for (auto& mesh: meshes) {
        mesh->render(shader);
    }

    for (auto& child:children) { 
        child->render(shader);
    }
}

bool Node::attached() {
    return !parent.has_value();
}

unique_ptr<Node> Node::disattachFromParent() {
    return parent.value().get().disattachChild(*this);
}

unique_ptr<Node> Node::disattachChild(Node& ch) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i].get() == &ch) {
            ch.transform = ch.getTotalTransform();
            auto chsh = move(children[i]);
            chsh->parent.reset();
            children.erase(children.begin() + i);

            return chsh;
        }
    }

    assert(false && "must exist");
    return {nullptr};
}

vector<reference_wrapper<Node>> Node::getChildren() const {
    auto v = vector<reference_wrapper<Node>> {};
    v.reserve(children.size());

    for (auto& ch : children) {
        v.push_back(ref(*ch.get()));
    }

    return v;
}
