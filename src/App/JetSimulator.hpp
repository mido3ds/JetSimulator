#pragma once
#include "App.hpp"
#include <Shaders/PhongShader.hpp>
#include <Cameras/ModelTrackingCamera.hpp>
#include <Models/Jet.hpp>

class JetSimulator: public App {
private:
    PhongShader* phongShader;
    Jet* jet;
    ModelTrackingCamera* camera;
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(double dt);
    virtual void onDraw(); 
};