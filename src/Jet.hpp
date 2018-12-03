#pragma once
#include "Model.hpp"

#define JET_MODEL_PATH "assets/models/su27/su27.dae"

class Jet: public Model {
private:
public:
    Jet();
    virtual void update(double dT) override;
    virtual void draw() override;
};