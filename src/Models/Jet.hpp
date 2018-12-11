#pragma once
#include <Scene/Model.hpp>
#include <Shaders/PhongShader.hpp>
#include <App/App.hpp>

#define JET_MODEL_PATH "assets/models/f14/f14GearUp.dae"
#define ROTATE_SPEED -0.54906f
#define YAW_MAX 0.0764f

class Jet: public Model {
private:
    App* app;
public:
    Jet();
    virtual void update(float dt) override;
};