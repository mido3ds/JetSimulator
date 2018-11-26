#pragma once
#include "Application.hpp"

class JetSimulator: public Application {
private:
public:
    virtual Config getConfig();
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(double dt);
    virtual void onDraw(); 
};