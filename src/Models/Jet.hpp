#pragma once
#include <Scene/Model.hpp>
#include <Shaders/PhongShader.hpp>

#define JET_MODEL_PATH "assets/models/su27/su27.dae"

class Jet: public Model {
private:
public:
    Jet();
    virtual void update(double dt) override;
    virtual void draw(PhongShader& shader) override;
};