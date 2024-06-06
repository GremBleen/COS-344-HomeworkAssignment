#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

const unsigned int SCR_CAM_WIDTH = 1000;
const unsigned int SCR_CAM_HEIGHT = 1000;

using namespace glm;

enum camera_mov {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Camera paramas
const float SPEED = 0.05f;
const float SENSITIVITY = 0.8f;
const float ZOOM = 45.0f;

class Camera {
public:
    // Camera-coord system
    vec3 position;
    vec3 view_vector;
    vec3 up_vector;
    mat4 cameraMatrix;

    bool first_mouse = true;

    // Camera options
    float movement_speed;
    float mouse_sens;
    float zoom;

    // Constructor using vecs
    Camera(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 view = vec3(0.0f, 0.0f, -1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float speed = SPEED, float sens = SENSITIVITY, float zoom = ZOOM) {
        this->position = pos;
        this->view_vector = view;
        this->up_vector = up;
        this->movement_speed = SPEED;
        this->mouse_sens = SENSITIVITY;
        this->zoom = ZOOM;
    }

    // get view matrix
    void viewMatrix(float near_plane, float far_plane) {
        // Initializes matrices since otherwise they will be the null matrix
        mat4 view = mat4(1.0f);
        mat4 projection = mat4(1.0f);

        // Makes camera look in the right direction from the right position
        view = lookAt(position, position + view_vector, up_vector);
        // Adds perspective to the scene
        projection = perspective(radians(zoom), (float)(SCR_CAM_WIDTH / SCR_CAM_HEIGHT), near_plane, far_plane);

        // Sets new camera matrix
        cameraMatrix = projection * view;
    }

    void setShaderMat4(Shader& shader, const char* uniform) {
        shader.use();
        shader.setMat4(uniform, cameraMatrix);
    }

    // process input
    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += movement_speed * view_vector;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position += movement_speed * -cross(view_vector, up_vector);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position += movement_speed * -view_vector;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += movement_speed * cross(view_vector, up_vector);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            view_vector = rotate(view_vector, movement_speed, up_vector);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            view_vector = rotate(view_vector, -movement_speed, up_vector);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            position += movement_speed * up_vector;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            position += movement_speed * -up_vector;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            movement_speed = SPEED * 1.2f;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            movement_speed = SPEED / 1.2f;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (first_mouse) {
                glfwSetCursorPos(window, (float)(SCR_CAM_WIDTH) / 2, (float)(SCR_CAM_HEIGHT) / 2);
                first_mouse = false;
            }

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            float xrot = mouse_sens * (float)(ypos - (float)(SCR_CAM_HEIGHT) / 2) / (float)(SCR_CAM_HEIGHT);
            float yrot = mouse_sens * (float)(xpos - (float)(SCR_CAM_WIDTH) / 2) / (float)(SCR_CAM_WIDTH);

            vec3 new_view = rotate(view_vector, radians(-xrot), normalize(cross(view_vector, up_vector)));

            if (!(angle(new_view, up_vector) <= radians(5.0f) || angle(new_view, -up_vector) <= radians(5.0f))) {
                view_vector = new_view;
            }

            view_vector = rotate(view_vector, radians(-yrot), up_vector);

            glfwSetCursorPos(window, (float)(SCR_CAM_WIDTH) / 2, (float)(SCR_CAM_HEIGHT) / 2);
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            first_mouse = true;
        }
    }

    // process mouse scroll
    void processMouseScroll(float yoffset) {
        if (zoom >= 1.0f && zoom <= 45.0f) {
            zoom -= yoffset;
        }
        if (zoom <= 1.0f) {
            zoom = 1.0f;
        }
        if (zoom >= 45.0f) {
            zoom = 45.0f;
        }
    }
};

#endif