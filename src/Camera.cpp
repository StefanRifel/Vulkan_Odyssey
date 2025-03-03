#include "Camera.h"

/**
 * @brief Verwaltet eine virtuelle Kamera in einer 3D-Szene und stellt Methoden zur Steuerung von Position, Orientierung, Perspektive und Zoom bereit.
 */
Camera::Camera()
    : yaw {DEFAULT_YAW}, pitch {DEFAULT_PITCH}, movementSpeed {DEFAULT_SPEED}, mouseSensitivity {DEFAULT_SENSITIVITY},  fieldOfView {FOV} {
    this->position = {0.0f, 1.0f, 2.0f};
    this->front = {0.0f, 0.0f, -1.0f};
    this->worldUp = {0.0f, 1.0f, 0.0f};
    
    updateCameraVectors();
}

/**
 * @brief Berechnet die View- und Projection-Matrix der Kamera und speichert sie in den entsprechenden Variablen.
 */
void Camera::look(uint32_t width, uint32_t height) {
    // Setzt die View-Matrix im Shader
    glm::vec3 pf = position + front;
    this->view = glm::lookAt(position, pf, up);

    // Setzt die Projection-Matrix im Shader
    this->perspective = glm::perspective(glm::radians(fieldOfView), width / (float) height, NEAR_PLANE, FAR_PLANE);
    // Ändert die Culling-Richtung
    this->perspective[1][1] *= -1;
}

void Camera::processKeyboard(Camera_Movement direction, float cDeltaTime) {

    float velocity = movementSpeed * cDeltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

/**
 * @brief Verarbeitet Mausbewegungen, um die Kamera zu drehen.
 * 
 * @param xPos Die neue X-Position der Maus.
 * @param yPos Die neue Y-Position der Maus.
 * @param constrainPitch Gibt an, ob der Neigungswinkel (Pitch) begrenzt werden soll, um ein Überschlagen der Kamera zu verhindern.
 */
void Camera::processMouseMovement(float xPos, float yPos, bool constrainPitch = true) {

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Begrenzen des Neigungswinkels
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::zoomWithMouseScroll(float yOffset) {
    fieldOfView -= (float)yOffset;
    if (fieldOfView < 45.0f) {
        fieldOfView = 45.0f;
    }
    if (fieldOfView > 120.0f) {
        fieldOfView = 120.0f;
    }
}

glm::mat4& Camera::getView() {
    return view;
}

const glm::mat4& Camera::getPerspective() const {
    return perspective;
}

const glm::vec3 &Camera::getPosition() const {
    return position;
}

const glm::vec3 &Camera::getFront() const {
    return front;
}
