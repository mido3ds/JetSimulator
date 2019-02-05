#pragma once
#include "App.hpp"
#include <Shaders/PhongShader.hpp>
#include <Cameras/ModelTrackingCamera.hpp>
#include <Models/Jet.hpp>
#include <Scene/Light.hpp>
#include <Models/SkyBox.hpp>
#include <Renderer/Renderer.h>
#include <Renderer/Effect.h>

class JetSimulator: public App {
private:
    PhongShader* phongShader;
    Jet* jet;
    Model* land;
    ModelTrackingCamera* camera;
    SkyBox* skybox;
    FadeOutTransition* fadeoutTran;
    FadeInTransition* fadeinTran;
    
    Renderer* renderer;

    Effect *inverseEffect, *grayscaleEffect, 
        *sepiaEffect, *vignetteEffect, *blurEffect;
	bool useFog = false, useSepia = false,
        useGrayscale = false, useVignette = false, 
        useBlur = false;

    DirLight sun {
        glm::vec3(0, 0, -1), // dir
        glm::vec3(0.1, 0.1, 0.1), // ambient
        glm::vec3(0.8, 0.8, 0.8), // diffuse
        glm::vec3(0.2f, 0.2f, 0.2f) // specular
    };
public:
    virtual Config getConfig() override;
    virtual void onCreate() override;
    virtual void onDestroy() override;
    virtual void onUpdate(float dt) override;
    virtual void onDraw() override; 
    virtual void onKeyPressed(int key, int modifierKey) override;
    virtual void onKeyReleased(int key, int modifierKey) override;
    virtual void onOpenglDebug(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, std::string message
    ) override;
    virtual void onError(int code, std::string msg) override;
};