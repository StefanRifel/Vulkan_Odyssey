#include "Window.h"

Window::Window(const std::string& windowName, int width, int height)
    : glfwWindow(nullptr), windowName(windowName), width(width), height(height) {
    initWindow();
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
    std::cout << "Window destroyed" << std::endl;
}

void Window::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);      //Aktieviert Anpassbarkeit der Fenstergröße
 
    glfwWindow = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

    if(!glfwWindow) {
        throw std::runtime_error("WINDOW::INITWINDOW::22 - glfwWindow konnte nicht erstellt werden");
    }
    
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);      //Callback für Fenstergrößenänderung
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(glfwWindow);
}

void Window::pollEvents() const {
    glfwPollEvents();
}

GLFWwindow* Window::getGLFWwindow() {
    return glfwWindow;
}

bool Window::getFramebufferResized() {
    return framebufferResized;
}

void Window::setFramebufferResized(bool b) {
    framebufferResized = b;
}

void Window::framebufferResizeCallback(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->setFramebufferResized(true);
}
