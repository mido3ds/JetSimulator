#include "Jet.hpp"

Jet::Jet() :Model(JET_MODEL_PATH) {}

void Jet::update(double dt) {

}

void Jet::draw(PhongShader& shader) {
    rootNode->draw(shader);
}
