#pragma once
#include "AbstractApp.hpp"
#include <Shaders/PhongShader.hpp>

class JetSimulator: public AbstractApp {
private:
    PhongShader* phongShader;
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(double dt);
    virtual void onDraw(); 
};