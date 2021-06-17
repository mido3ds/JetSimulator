#pragma once
#include <Scene/Model.hpp>
#include <Shaders/PhongShader.hpp>
#include <App/App.hpp>
#include "Missile.hpp"
#include <vector>
#include <memory>

using namespace std;

#define JET_MODEL_PATH "assets/models/f14/f14GearUp.dae"
#define JET_SPEED 300
#define ROTATE_SPEED -0.54906f
#define YAW_MAX 0.0764f

#define MISSILE_SPD JET_SPEED*1.5
#define MISSILE_TIME 40.0f
#define NUM_MISSILES 8
#define MISSILE_NAME "missile"

class Jet: public Model {
private:
    App* app;
    vector<Missile> missiles;
    int missileToFire = 0;
    float speed = JET_SPEED;
public:
    Jet();

    virtual void update(float dt) override;
    virtual void render(unique_ptr<PhongShader>& shader) override;
    void fireMissile();
};
