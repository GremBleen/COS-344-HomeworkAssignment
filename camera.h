#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

enum camera_mov {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Camera paramas
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    // Camera-coord system
    vec3 position;
    vec3 front_vector;
    vec3 up_vector;
    vec3 right_vector;
    vec3 world_up_vector;

    // Euler angles
    float yaw;
    float pitch;

    // Camera options
    float movement_speed;
    float mouse_sens;
    float zoom;

    // Constructor using vecs
    Camera(
        vec3 position = vec3(0.0f, 0.0f, 0.0f),
        vec3 up = vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH
    ) {
        front_vector = vec3(0.0f, 0.0f, -1.0f);
        movement_speed = SPEED;
        mouse_sens = SENSITIVITY;
        zoom = ZOOM;
        position = position;
        world_up_vector = up;
        yaw = yaw;
        pitch = pitch;
        updateCamVectors();
    }

    // Constructor using scalars
    Camera(
        float pos_x,
        float pos_y,
        float pos_z,
        float up_x,
        float up_y,
        float up_z,
        float yaw,
        float pitch
    ) {
        front_vector = vec3(0.0f, 0.0f, -1.0f);
        movement_speed = SPEED;
        mouse_sens = SENSITIVITY;
        zoom = ZOOM;
        position = vec3(pos_x, pos_y, pos_z);
        world_up_vector = vec3(up_x, up_y, up_z);
        yaw = yaw;
        pitch = pitch;
        updateCamVectors();
    }

    // get view matrix
    mat4 viewMatrix() {
        return lookAt(position, position + front_vector, up_vector);
    }

    // porcess keyboard input to move camera
    void processInput(camera_mov mov_dir, float delta_t) {
        float velocity = movement_speed * delta_t;
        if (mov_dir == FORWARD) {
            position += front_vector * velocity;
        }
        if (mov_dir == BACKWARD) {
            position -= front_vector * velocity;
        }
        if (mov_dir == LEFT) {
            position -= right_vector * velocity;
        }
        if (mov_dir == RIGHT) {
            position += right_vector * velocity;
        }
    }

    // process mouse input to move camera
    void processInput(float x_off, float y_off, GLboolean pitch = true) {
        x_off *= mouse_sens;
        y_off *= mouse_sens;

        yaw += x_off;
        pitch += y_off;

        if (pitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateCamVectors();
    }

    // process mouse scroll to zoom camera
    void processInput(float y_off) {
        if (zoom >= 1.0f && zoom <= 45.0f) {
            zoom -= y_off;
        }
        if (zoom <= 1.0f) {
            zoom = 1.0f;
        }
        if (zoom >= 45.0f) {
            zoom = 45.0f;
        }
    }

private:
    void updateCamVectors() {
        // calc front vector
        vec3 front;
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front_vector = normalize(front);
        right_vector = normalize(cross(front_vector, world_up_vector));
        up_vector = normalize(cross(right_vector, front_vector));
    }
};

#endif