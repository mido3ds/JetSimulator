#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "Mesh.hpp"
#include <Shaders/PhongShader.hpp>

class Node {
private:
    static Node* build(const aiScene* scene, const aiNode* node, Node* parent, std::vector<Mesh*>& allMeshes);
    Node* parent;
    std::vector<Node*> children;
    std::vector<Mesh*> meshes;
public:
    static Node* build(const aiScene* scene, const aiNode* node, std::vector<Mesh*>& allMeshes);

    Node(const std::string& name, const glm::mat4& transform, Node *parent, 
        const std::vector<Node*>& children, const std::vector<Mesh*>& meshes);
    ~Node();

    glm::mat4 getTotalTransform() const;
    Node* getNodeByName(const std::string& name);
    void draw(PhongShader& shader);

    const std::string name;
    glm::mat4 transform;

    std::vector<Node*> getChildren();
    std::vector<Mesh*> getMeshes();
    Node* getParent();
    void addToParent(Node* parent);
    void removeFromParent();
    void removeChild(Node* ch);
    bool attached();
};