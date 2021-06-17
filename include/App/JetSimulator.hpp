#pragma once
#include <App/App.hpp>
#include <Shaders/PhongShader.hpp>
#include <Cameras/ModelTrackingCamera.hpp>
#include <Models/Jet.hpp>
#include <Scene/Light.hpp>
#include <Shaders/Effects.hpp>
#include <Models/SkyBox.hpp>
#include <memory>

using namespace std;

class JetSimulator: public App {
private:
    unique_ptr<PhongShader> phongShader;
    unique_ptr<Jet> jet;
    unique_ptr<Model> land;
    unique_ptr<ModelTrackingCamera> camera;
    unique_ptr<SkyBox> skybox;

    Effects effects;

    const DirLight sun {
        dir: glm::vec3(0, 0, -1),
        ambient: glm::vec3(0.1, 0.1, 0.1),
        diffuse: glm::vec3(0.8, 0.8, 0.8),
        specular: glm::vec3(0.2f, 0.2f, 0.2f),
    };
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(float dt);
    virtual void onRender(); 
    virtual void onKeyPressed(int key, int modifierKey);
    virtual void onKeyReleased(int key, int modifierKey);
};
