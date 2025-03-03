#ifndef CAMERA_HH
#define CAMERA_HH

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <cmath>

/**
 * @enum Camera_Movement
 * @brief Richtungen, in die sich die Kamera bewegen kann.
 */
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Standardwerte für Kameraeinstellungen
const float FOV = 60.0f;
const float ASPECT_RATIO = 1024.0f / 768.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

const float DEFAULT_YAW         = -90.0f;
const float DEFAULT_PITCH       = 0.0f;

// Standardbewegungswerte
const float DEFAULT_SPEED       =  2.5f;
const float DEFAULT_SENSITIVITY =  0.1f;

/**
 * @class Camera
 * @brief Eine Kamera-Klasse, die Eingaben verarbeitet und die entsprechenden Euler-Winkel, Vektoren und Matrizen für die 3D-Ansicht berechnet.
 *
 * Die Kamera-Klasse ermöglicht die Steuerung der Position, Orientierung und Bewegung der Kamera. Sie verarbeitet Eingaben von Tastatur und Maus, 
 * um die Kamera zu bewegen und zu drehen, und stellt Methoden zur Verfügung, um die View- und Projektions-Matrizen für das Rendering zu erhalten.
 *
 * Beispiel einer vereinfachten Darstellung der Kamerakoordinaten:
 *
 *       +Y  (nach oben)
 *        |
 *        |
 *        |
 *        O-------> +X (rechts)
 *       /
 *      /
 *     v +Z (nach vorne)
 *
 * Die Kamera blickt standardmäßig in Richtung der positiven Z-Achse und kann sich im 3D-Raum bewegen und rotieren.
 */
class Camera {

private:
    glm::vec3 position;                  ///< Kameraposition im Weltkoordinatensystem
    glm::vec3 front;                     ///< Blickrichtung der Kamera
    glm::vec3 up;                        ///< "Oben"-Richtung der Kamera
    glm::vec3 right;                     ///< "Rechts"-Richtung der Kamera
    glm::vec3 worldUp;                   ///< "Oben"-Richtung in der Welt

    glm::mat4 view;                      ///< View-Matrix der Kamera
    glm::mat4 perspective;               ///< Projektions-Matrix der Kamera

    // Kamera-Rotation
    float yaw;                           ///< Yaw-Winkel (horizontale Drehung)
    float pitch;                         ///< Pitch-Winkel (vertikale Drehung)

    // Kameraoptionen
    float movementSpeed;                 ///< Geschwindigkeit der Kamerabewegung
    float mouseSensitivity;              ///< Empfindlichkeit der Mausbewegung
    float fieldOfView;                   ///< Sichtfeld (Zoom-Level)

    float lastX = 1024.0f / 2.0f;             ///< Letzte X-Position der Maus
    float lastY = 768.0f / 2.0f;            ///< Letzte Y-Position der Maus

    void updateCameraVectors() {
        // Berechnung des neuen Front-Vektors
        glm::vec3 newFront;
        newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        newFront.y = std::sin(glm::radians(pitch));
        newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp)); // Normalisiere denn Vektor, da ihre Länge näher an 0 herankommt, je weiter man nach oben oder unten schaut, was zu einer langsameren Bewegung führt.
        up    = glm::normalize(glm::cross(right, front));
    }

public:
    float deltaTime = 0.0f;	            ///< Zeit zwischen dem aktuellen und dem letzten Frame
    float lastFrame = 0.0f;             ///< Zeitpunkt des letzten Frames

    Camera();
    void look(uint32_t width, uint32_t height);

    glm::mat4 &getView();
    const glm::mat4& getPerspective() const;

    const glm::vec3& getPosition() const;
    const glm::vec3& getFront() const;

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xPos, float yPos, bool constrainPitch);
    void zoomWithMouseScroll(float yOffset);
};

#endif