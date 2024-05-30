#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "model.h"
#include "mesh.h"
#include "shader.hpp"

#define timeDT std::chrono::_V2::steady_clock::time_point

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// sizes
const GLuint SCR_WIDTH = 1000, SCR_HEIGHT = 1000;

// camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char* getError() {
    const char* errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW() {
    glewExperimental = true; // Needed for core profile
    if (!glfwInit()) {
        throw getError();
    }
}

inline void startUpGLEW() {
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw getError();
    }
}

inline GLFWwindow* setUp() {
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow* window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "u22625462", NULL, NULL);
    if (window == NULL) {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // TODO: Add stbi_set_flip_vertically_on_load(true) to the code
    // stbi_set_flip_vertically_on_load(true);

    startUpGLEW();
    return window;
}

int main() {
    GLFWwindow* window;
    try {
        window = setUp();
    } catch (const char* e) {
        cout << e << endl;
        throw;
    }

    int* xpos = new int;
    int* ypos = new int;
    int* width = new int;
    int* height = new int;
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), xpos, ypos, width, height);
    glfwSetWindowPos(window, *xpos + (*width - 1000) / 2, *ypos + (*height - 1100) / 2);

    glEnable(GL_DEPTH_TEST);

    Shader sceneShader("shader.vs", "shader.fs");

    // SHAPES OR SCENE STUFF GO HERE

    do {
        // time logic, per frame
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // any input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // enable shader
        sceneShader.use();

        // camera/view transformation
        mat4 proj = perspective(radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        mat4 view = camera.viewMatrix();
        sceneShader.setMat4("proj", proj);
        sceneShader.setMat4("view", view);

        // render shapes here
        // ourModel.Draw(sceneShader);
    } while (!glfwWindowShouldClose(window));
}
