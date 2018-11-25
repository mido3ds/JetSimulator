#ifndef SCENE_HPP
#define SCENE_HPP

class Application;

class Scene {
private:
    Application* application;

    //Delete copy constructor and assignment operation
    Scene(Scene const &) = delete;
    Scene & operator =(Scene const &) = delete;

protected:

    Application* getApplication() { return application; }

public:
    Scene(Application* application): application(application) {}

    //Called once before the application loop starts
    virtual void Initialize();

    //Called every frame to update scene
    //  delta_time: time elapsed between the last and current fame (in seconds)
    virtual void Update(double delta_time);

    //Called every frame to draw scene 
    virtual void Draw();

    //Called once after the application loop ends
    virtual void Finalize();
};

#endif