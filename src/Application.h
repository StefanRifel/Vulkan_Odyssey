#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Scene.h"
#include "InputManager.h"
#include "Window.h"

class Application {

private:
    std::unique_ptr<Window> window;                 // Fensterverwaltung
    std::unique_ptr<Scene> scene;                   // Vulkan-Rendering-Logik
    std::unique_ptr<InputManager> inputManager;     // Eingabeverwaltung

    void init();
    void processInput();
    
public:
    Application();
    ~Application();
    void run();
};

#endif