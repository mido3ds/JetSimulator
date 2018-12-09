#pragma once
#include "App.hpp"
#include <Shaders/PhongShader.hpp>
#include <Cameras/ModelTrackingCamera.hpp>
#include <Models/Jet.hpp>
#include <Scene/Light.hpp>
#include <Models/SkyBox.hpp>

class JetSimulator: public App {
private:
    PhongShader* phongShader;
    Jet* jet;
    ModelTrackingCamera* camera;
    SkyBox* skybox;
    DirLight sun {
        glm::vec3(0, -1, 0), // dir
        glm::vec3(0.1, 0.1, 0.1), // ambient
        glm::vec3(0.8, 0.8, 0.8), // diffuse
        glm::vec3(0.2f, 0.2f, 0.2f) // specular
    };
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(float dt);
    virtual void onDraw(); 
};