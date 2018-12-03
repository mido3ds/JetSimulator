#pragma once
#include "Application.hpp"
#include "PhongShader.hpp"

class JetSimulator: public Application {
private:
    PhongShader* phongShader;
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(double dt);
    virtual void onDraw(); 
};