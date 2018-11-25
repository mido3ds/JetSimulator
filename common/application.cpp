#include "application.hpp"

#include <shader.hpp>
#include <scene.hpp>
#include <iostream>

#include "mesh.hpp"

Application::Application(const char* title, int width, int height, bool isFullscreen){

    //Try to initialize GLFW
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Set the opengl version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Use only opengl core feature (Ignore compatibility with older versions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Make window size fixed (User can't resize it)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    //Set Number of sample used in MSAA (0 = Disabled)
    glfwWindowHint(GLFW_SAMPLES, 0);

    //Enable Double Buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    //Set the bit-depths of the frame buffer
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    //Set the refresh rate of the window (GLFW_DONT_CARE = Run as fast as possible)
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    //If the application is fullscreen, set the monitor to primary monitor
    //otherwise set the monitor to null (windowed)
    GLFWmonitor *monitor = isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, title, monitor, nullptr);
    
    //Check if window was created
    if(!window){
        std::cerr << "Failed to create Window" << std::endl;
        glfwTerminate(); //Remember to terminate GLFW
        exit(EXIT_FAILURE);
    }

    //Set the window to be the current OpenGL context 
    glfwMakeContextCurrent(window);
    //Load OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window); //Remember to destroy window
        glfwTerminate(); //Remember to terminate GLFW
        exit(EXIT_FAILURE);
    }
}

Application::~Application(){
    //Destroy window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::setScene(Scene* scene){
    this->currentScene = scene;
}

void Application::run(){
    
    //Main application loop:
    //- Initialize Scene
    //- While window still open, do:
    //      - Update Scene
    //      - Draw Scene
    //      - Swap window buffers and poll input events (keyboard, mouse, etc)
    //- Finalize Scene

    currentScene->Initialize();
    
    double lastFrame = glfwGetTime();

    while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)){
        
        double currentFrame = glfwGetTime();

        currentScene->Update(currentFrame - lastFrame); //Send time difference between last and current frame
        
        currentScene->Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastFrame = currentFrame;
    }

    currentScene->Finalize();

}