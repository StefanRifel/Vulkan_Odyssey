#include "Application.hpp"

void Application::run() {
    vulkanContext.init();
    vulkanContext.mainLoop();
    vulkanContext.cleanup();
}