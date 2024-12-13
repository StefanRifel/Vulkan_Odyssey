#include "Application.hpp"

void Application::run() {
    scene.init();
    scene.mainLoop();
    scene.cleanup();
}