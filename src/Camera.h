#ifndef CAMERA_HH
#define CAMERA_HH

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "base/SwapChain.h"

#include <cmath>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Constants for default camera settings
const float FOV = 60.0f;
const float ASPECT_RATIO = 1024.0f / 768.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

const float DEFAULT_YAW         = -90.0f;
const float DEFAULT_PITCH       = 0.0f;

// Default movement values
const float DEFAULT_SPEED       =  2.5f;
const float DEFAULT_SENSITIVITY =  0.1f;

/**
     * @class Camera
     * @brief A camera class that processes input and calculates the corresponding Euler Angles, Vectors, and Matrices for use in OpenGL.
     *
     * The Camera class handles all the operations related to the camera's position, orientation, and movement.
     * It processes keyboard and mouse input to update the camera's position and orientation and provides methods to get view and projection matrices for rendering.
     *
     * Here is a simple ASCII representation of the camera setup:
     *
     *       +Y  (up)
     *        |
     *        |
     *        |
     *        O-------> +X (Right)
     *       /
     *      /
     *     v +Z (Forward)
     *
     * The camera looks in the direction of the positive Z-axis by default, and can move and rotate in the 3D space.
 */
class Camera {

private:
    glm::vec3 position;                  ///< Camera position in world space
    glm::vec3 front;                     ///< Camera front direction
    glm::vec3 up;                        ///< Camera up direction
    glm::vec3 right;                     ///< Camera right direction
    glm::vec3 worldUp;                   ///< World's up direction

    glm::mat4 view;
    glm::mat4 perspective;

    // Camera rotation
    float yaw;                      ///< Camera yaw (horizontal rotation)
    float pitch;                    ///< Camera pitch (vertical rotation)

    // Camera options
    float movementSpeed;            ///< Camera movement speed
    float mouseSensitivity;         ///< Mouse sensitivity for looking around
    float fieldOfView;              ///< Field of view for zoom

    float lastX = 1024.0f / 2;      ///< Last X position of the mouse
    float lastY = 768.0f / 2;       ///< Last Y position of the mouse

    void updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 newFront;
        newFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        newFront.y = std::sin(glm::radians(pitch));
        newFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }

public:
    float deltaTime = 0.0f;	        ///< Time between current frame and last frame
    float lastFrame = 0.0f;         ///< Time of the last frame

    bool flashlight;

    Camera();
    void look();

    glm::mat4 &getView();
    const glm::mat4& getPerspective() const;

    const glm::vec3& getPosition() const;
    const glm::vec3& getFront() const;

    bool isFlashlight();

    void setFlashlight(bool flashlight);

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xPos, float yPos, bool constrainPitch);
    void zoomWithMouseScroll(float yOffset);
};

#endif