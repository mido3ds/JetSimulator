#pragma once
#include <Scene/Model.hpp>
#include <Shaders/PhongShader.hpp>
#include <App/App.hpp>

#define JET_MODEL_PATH "assets/models/su27/su27e.dae"
#define ROTATE_SPEED -0.54906f
#define YAW_MAX -2.54906f

class Jet: public Model {
private:
    App* app;
public:
    Jet();
    virtual void update(float dt) override;
};