#include "InputManager.h"

InputManager::InputManager(Window* window) : window(window) {
    glfwSetWindowUserPointer(window->getGLFWwindow(), this);

    glfwSetKeyCallback(window->getGLFWwindow(), keyCallback);
    glfwSetMouseButtonCallback(window->getGLFWwindow(), mouseButtonCallback);
    glfwSetCursorPosCallback(window->getGLFWwindow(), cursorPositionCallback);
    glfwSetScrollCallback(window->getGLFWwindow(), scrollCallback);
}

InputManager::~InputManager() {
    glfwSetWindowUserPointer(window->getGLFWwindow(), nullptr);
}

/**
 * @brief Überprüft, ob eine bestimmte Taste gedrückt ist.
 * 
 * @param key Der GLFW-Schlüsselcode der zu überprüfenden Taste.
 * @return true, wenn die Taste gedrückt ist, andernfalls false.
 */
bool InputManager::isKeyPressed(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

/**
 * @brief Überprüft, ob eine bestimmte Maustaste gedrückt ist.
 * 
 * @param button Der GLFW-Schlüsselcode der zu überprüfenden Maustaste.
 * @return true, wenn die Maustaste gedrückt ist, andernfalls false.
 */
bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates.find(button);
    return it != mouseButtonStates.end() && it->second;
}

/**
 * @brief Gibt die aktuelle Position des Mauszeigers zurück.
 * 
 * @return Die aktuelle Position des Mauszeigers als `glm::vec2`.
 */
glm::vec2 InputManager::getMousePosition() const {
    return mousePosition;
}

/**
 * @brief Gibt den aktuellen Scroll-Offset der Maus zurück.
 * 
 * Nach dem Abrufen wird der Scroll-Offset auf (0, 0) zurückgesetzt.
 * 
 * @return Der Scroll-Offset der Maus als `glm::vec2`.
 */
glm::vec2 InputManager::getMouseScrollOffset() {
    glm::vec2 offset = mouseScrollOffset;
    mouseScrollOffset = glm::vec2(0.0f, 0.0f);
    return offset;
}

/**
 * @brief Callback-Funktion zur Verarbeitung von Tasteneingaben.
 * 
 * Diese Funktion wird aufgerufen, wenn eine Taste gedrückt, losgelassen oder wiederholt wird, 
 * und aktualisiert den Zustand der entsprechenden Taste im `InputManager`.
 * 
 * @param window Das GLFW-Fenster, das die Eingabe empfangen hat.
 * @param key Der Schlüsselcode der betroffenen Taste.
 * @param scancode Der hardware-spezifische Scan-Code (nicht genutzt).
 * @param action Die Art der Aktion (z. B. GLFW_PRESS, GLFW_RELEASE).
 * @param mods Modifizierende Tasten (z. B. Shift, Ctrl; nicht genutzt).
 */
void InputManager::keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            inputManager->keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            inputManager->keyStates[key] = false;
        }
    }
}


/**
 * @brief Callback-Funktion zur Verarbeitung von Maustasten-Eingaben.
 * 
 * Diese Funktion wird aufgerufen, wenn eine Maustaste gedrückt oder losgelassen wird, 
 * und aktualisiert den Zustand der entsprechenden Maustaste im `InputManager`.
 * 
 * @param window Das GLFW-Fenster, das die Eingabe empfangen hat.
 * @param button Der Schlüsselcode der betroffenen Maustaste.
 * @param action Die Art der Aktion (z. B. GLFW_PRESS, GLFW_RELEASE).
 * @param mods Modifizierende Tasten (z. B. Shift, Ctrl; nicht genutzt).
 */
void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        if (action == GLFW_PRESS) {
            inputManager->mouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            inputManager->mouseButtonStates[button] = false;
        }
    }
}

/**
 * @brief Callback-Funktion zur Verarbeitung von Mausbewegungen.
 * 
 * Diese Funktion wird aufgerufen, wenn sich die Position des Mauszeigers ändert, 
 * und aktualisiert die Mausposition im `InputManager`.
 * 
 * @param window Das GLFW-Fenster, das die Eingabe empfangen hat.
 * @param xpos Die neue X-Koordinate des Mauszeigers.
 * @param ypos Die neue Y-Koordinate des Mauszeigers.
 */
void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        inputManager->mousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

/**
 * @brief Callback-Funktion zur Verarbeitung von Mausradbewegungen.
 * 
 * Diese Funktion wird aufgerufen, wenn das Mausrad bewegt wird, 
 * und aktualisiert den Scroll-Offset im `InputManager`.
 * 
 * @param window Das GLFW-Fenster, das die Eingabe empfangen hat.
 * @param xoffset Die horizontale Scrollbewegung.
 * @param yoffset Die vertikale Scrollbewegung.
 */
void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        inputManager->mouseScrollOffset += glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}