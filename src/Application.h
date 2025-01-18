#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

#include "Scene.h"
#include "InputManager.h"
#include "Window.h"

/**
 * @class Application
 * @brief Hauptklasse zur Verwaltung der Anwendung.
 *
 * Diese Klasse steuert die Initialisierung, das Rendering und die Eingabeverarbeitung der Vulkan-Anwendung. 
 * Sie koordiniert die Fensterverwaltung, Szenenlogik und Benutzereingaben.
 */
class Application {

private:
    std::unique_ptr<Window> window;                 // Fensterverwaltung
    std::unique_ptr<Scene> scene;                   // Vulkan Rendering Logik
    std::unique_ptr<InputManager> inputManager;     // Eingabeverwaltung

    void init();
    void processInput();
    
public:
    static constexpr int WIDTH = 1024;
    static constexpr int HEIGHT = 768;
    static inline const std::string TITLE = "Vulkan Odyssey";

    Application();
    ~Application();

    void run();
};

#endif