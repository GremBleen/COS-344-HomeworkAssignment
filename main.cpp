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

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "light.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Window size
unsigned int SCR_WIDTH = 1000;
unsigned int SCR_HEIGHT = 1000;

// Camera
Camera camera(vec3(-8.5f, 0.2f, 0.0f), vec3(1.0f, 0.0f, 0.0f));

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const char* getError() {
    const char* errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW() {
    if (!glfwInit()) {
        throw getError();
    }
}

inline void startUpGLAD() {
    // glewExperimental = true; // Not needed with GLAD

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        exit(-1);
    }
}

inline GLFWwindow* setUp() {
    startUpGLFW();
    // Window Hints
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    GLFWmonitor* MyMonitor = glfwGetPrimaryMonitor(); // The primary monitor.. Later Occulus?..

    // glfw window
    GLFWwindow* window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Experiment", NULL, NULL);
    if (window == NULL) {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    startUpGLAD();

    // OpenGL options
    glClearColor(0.18f, 0.44f, 1.0f, 1.0f);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    // Define Lights here
    DirectionalLight sunLight = ld_lights::loadDirectionalLight();

    PointLight p1 = ld_lights::loadPointLight();
    PointLight p2 = ld_lights::loadPointLight();
    PointLight p3 = ld_lights::loadPointLight();
    PointLight p4 = ld_lights::loadPointLight();
    PointLight p5 = ld_lights::loadPointLight();
    PointLight p6 = ld_lights::loadPointLight();
    PointLight p7 = ld_lights::loadPointLight();
    PointLight p8 = ld_lights::loadPointLight();
    PointLight p9 = ld_lights::loadPointLight();
    PointLight p10 = ld_lights::loadPointLight();
    PointLight p11 = ld_lights::loadPointLight();
    PointLight p12 = ld_lights::loadPointLight();
    PointLight p13 = ld_lights::loadPointLight();
    PointLight p14 = ld_lights::loadPointLight();
    PointLight p15 = ld_lights::loadPointLight();
    PointLight p16 = ld_lights::loadPointLight();
    PointLight p17 = ld_lights::loadPointLight();
    PointLight p18 = ld_lights::loadPointLight();
    PointLight p19 = ld_lights::loadPointLight();
    PointLight p20 = ld_lights::loadPointLight();

    PointLight pointLights[20] = {
        p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
        p11, p12, p13, p14, p15, p16, p17, p18, p19, p20
    };

    SpotLight drone_light = ld_lights::loadSpotLight();

    // Build and compile our shader program
    Shader defaultShader("Shaders/defVertexShader.glsl", "Shaders/defFragmentShader.glsl");
    Shader lightShader("Shaders/lightVertexShader.glsl", "Shaders/lightFragmentShader.glsl");
    // Shader glassShader("Shaders/defVertexShader.glsl", "Shaders/glassFragmentShader.glsl");

    vec3 dirLightPos = vec3(0.0f, 10.0f, 0.0f);

    float light_y = 4.0f;
    float light_z = 3.0f;
    float light_x_start = -10.0f;
    float light_x_offset = 1.2f;

    vec3 spotPos[20] = {
        vec3(light_x_start + (light_x_offset * 0), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 1), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 2), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 3), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 4), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 5), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 6), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 7), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 8), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 9), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 10), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 11), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 12), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 13), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 14), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 15), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 16), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 17), light_y, -light_z),
        vec3(light_x_start + (light_x_offset * 18), light_y, light_z),
        vec3(light_x_start + (light_x_offset * 19), light_y, -light_z),
    };
    vec3 droneLightPos = vec3(0.0f, 0.0f, 0.0f);

    mat4 dirLightModel = mat4(1.0f);
    mat4 pointModels[20] = {
        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),

        mat4(1.0f),
        mat4(1.0f),
    };

    mat4 droneLightModel = mat4(1.0f);

    sunLight.colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 20; i++) {
        pointLights[i].colour = vec3(1.0f, 1.0f, 1.0f);
    }
    drone_light.colour = vec4(1.0f, 1.0f, 1.0f, 0.0f);

    dirLightModel = translate(dirLightModel, dirLightPos);
    sunLight.position = dirLightPos;

    for (int i = 0; i < 20; i++) {
        pointModels[i] = translate(pointModels[i], spotPos[i]);
        pointModels[i] = scale(pointModels[i], vec3(0.1f, 0.1f, 0.1f));
        pointLights[i].position = spotPos[i];
    }

    // Define Shapes here
    Model* eastWall = new Model(ld_shp::loadEastWall());
    Model* eastSideGlass1 = new Model(ld_shp::loadEastGlassWall());
    Model* eastSideGlass1_1 = new Model(ld_shp::loadEastGlassWall());
    Model* eastSideGlass2 = new Model(ld_shp::loadEastGlassWall());
    Model* eastSideGlass2_1 = new Model(ld_shp::loadEastGlassWall());
    Model* floor = new Model(ld_shp::loadFloor(30.0f, 7.0f, ld_tex::loadGreyUpholstery()));
    // Model* drone = new Model(ld_shp::loadDrone()); TODO: Implement drone
    Model* longWall1 = new Model(ld_shp::loadLongWall(ld_tex::loadConcrete()));
    Model* longWall2 = new Model(ld_shp::loadLongWall(ld_tex::loadConcrete()));
    Model* woodPanel1 = new Model(ld_shp::loadWoodPanel(ld_tex::loadPlanksTexture()));
    Model* woodPanel2 = new Model(ld_shp::loadWoodPanel(ld_tex::loadPlanksTexture()));
    Model* westWall = new Model(ld_shp::loadWestWall());

    Model* chair1 = new Model(ld_shp::loadChair1(ld_tex::loadGreyUpholstery()));
    Model* chair2 = new Model(ld_shp::loadChair1(ld_tex::loadGreyUpholstery()));
    Model* Table1 = new Model(ld_shp::loadTable1(ld_tex::loadSpeckledCounter()));
    Model* Table2 = new Model(ld_shp::loadTable2(ld_tex::loadSpeckledCounter()));

    Model* roof = new Model(ld_shp::loadRoof());

    int num_grid_west = 14;
    vector<Model*> westGrid1;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid1.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid2;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid2.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid3;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid3.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid4;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid4.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid5;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid5.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid6;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid6.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid7;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid7.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid8;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid8.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid9;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid9.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid10;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid10.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid11;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid11.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> westGrid12;
    for (int i = 0; i < num_grid_west; i++) {
        westGrid12.push_back(new Model(ld_shp::loadGridCross()));
    }

    int num_east_grid = 14;
    vector<Model*> eastGrid1;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid1.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid2;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid2.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid3;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid3.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid4;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid4.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid5;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid5.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid6;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid6.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid7;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid7.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid8;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid8.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid9;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid9.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid10;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid10.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid11;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid11.push_back(new Model(ld_shp::loadGridCross()));
    }
    vector<Model*> eastGrid12;
    for (int i = 0; i < num_east_grid; i++) {
        eastGrid12.push_back(new Model(ld_shp::loadGridCross()));
    }

    // OBJECT MODELS
    mat4 floorModel = mat4(1.0f);
    mat4 eastWallModel = mat4(1.0f);
    mat4 eastSideGlass1Model = mat4(1.0f);
    mat4 eastSideGlass1_1Model = mat4(1.0f);
    mat4 eastSideGlass2Model = mat4(1.0f);
    mat4 eastSideGlass2_1Model = mat4(1.0f);
    mat4 longWall1Model = mat4(1.0f);
    mat4 longWall2Model = mat4(1.0f);
    mat4 woodPanel1Model = mat4(1.0f);
    mat4 woodPanel2Model = mat4(1.0f);
    mat4 westGrid1Model = mat4(1.0f);
    mat4 westWallModel = mat4(1.0f);

    vector<mat4> westGridModals(num_grid_west, mat4(1.0f));
    vector<mat4> eastGridModals(num_east_grid, mat4(1.0f));

    mat4 tab1Model = mat4(1.0f);
    mat4 tab2Model = mat4(1.0f);
    mat4 chair1Model = mat4(1.0f);
    mat4 chair2Model = mat4(1.0f);

    mat4 roofModel = mat4(1.0f);

    // East wall manipulation
    eastWallModel = translate(eastWallModel, vec3(-15.0f, 3.5f, 0.0f));
    eastWallModel = rotate(eastWallModel, radians(90.0f), vec3(0.0f, 1.0f, 0.0f));

    // West wall manipulation
    westWallModel = translate(westWallModel, vec3(15.0f, 3.5f, 0.0f));
    westWallModel = rotate(westWallModel, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));

    eastSideGlass1Model = translate(eastSideGlass1Model, vec3(-14.8f, 3.5f, 1.65f));
    eastSideGlass1_1Model = translate(eastSideGlass1_1Model, vec3(-14.8f, 3.5f, 2.95f));
    eastSideGlass1Model = rotate(eastSideGlass1Model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    eastSideGlass1_1Model = rotate(eastSideGlass1_1Model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));

    eastSideGlass2Model = translate(eastSideGlass2Model, vec3(-14.8f, 3.5f, -1.65f));
    eastSideGlass2_1Model = translate(eastSideGlass2_1Model, vec3(-14.8f, 3.5f, -2.95f));
    eastSideGlass2Model = rotate(eastSideGlass2Model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    eastSideGlass2_1Model = rotate(eastSideGlass2_1Model, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));

    longWall1Model = translate(longWall1Model, vec3(0.0f, 3.5f, 3.5f));
    longWall2Model = translate(longWall2Model, vec3(0.0f, 3.5f, -3.5f));

    woodPanel1Model = translate(woodPanel1Model, vec3(0.0,1.2f, 3.0f));
    woodPanel1Model = rotate(woodPanel1Model, radians(15.0f), vec3(1.0f, 0.0f, 0.0f));
    woodPanel2Model = translate(woodPanel2Model, vec3(0.0, 1.2f, -3.0f));
    woodPanel2Model = rotate(woodPanel2Model, radians(-15.0f), vec3(1.0f, 0.0f, 0.0f));

    tab1Model = translate(tab1Model, vec3(-0.8f, 0.4f, 0.0f));
    tab1Model = scale(tab1Model, vec3(0.4f, 0.275f, 0.4f));

    tab2Model = translate(tab2Model, vec3(0.8f, 0.35f, 0.0f));
    tab2Model = scale(tab2Model, vec3(0.4f, 0.4f, 0.4f));

    chair1Model = translate(chair1Model, vec3(-0.8f, 0.25f, 0.8f));
    chair1Model = scale(chair1Model, vec3(0.4f, 0.4f, 0.4f));

    chair2Model = translate(chair2Model, vec3(0.8f, 0.25f, 0.8f));
    chair2Model = scale(chair2Model, vec3(0.4f, 0.4f, 0.4f));

    roofModel = translate(roofModel, vec3(0.0f, 7.0f, 0.0f));

    for (int i = 0; i < num_grid_west; i++) {
        westGridModals[i] = translate(westGridModals[i], vec3(14.8f, 3.5f, 0.0f));

        westGridModals[i] = translate(westGridModals[i], vec3(0.0f, 0.0f, -3.5f + (i * 0.5f)));

        westGridModals[i] = rotate(westGridModals[i], radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    }

    for (int i = 0; i < num_east_grid; i++) {
        eastGridModals[i] = translate(eastGridModals[i], vec3(-14.6f, 3.5f, 0.0f));

        eastGridModals[i] = translate(eastGridModals[i], vec3(0.0f, 0.0f, -3.5f + (i * 0.5f)));

        eastGridModals[i] = rotate(eastGridModals[i], radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    }

    defaultShader.use();
    defaultShader.setMat4("model", floorModel);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);
        camera.viewMatrix(0.1f, 100.0f);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ? model matrix is the transformation from the object's local space to world space
        // ? view matrix is the transformation from world space to camera space
        // ? projection matrix is the transformation from camera space to clip space

        defaultShader.use();

        // Directional Light
        sunLight.sendToShader(defaultShader);

        // Point Lights
        for (int i = 0; i < 20; i++) {
            pointLights[i].sendToShader(defaultShader, i);
        }

        // Spot Light
        drone_light.position = camera.position;
        drone_light.direction = camera.view_vector;
        drone_light.sendToShader(defaultShader);

        // Floor
        defaultShader.setMat4("model", floorModel);
        floor->Draw(defaultShader, camera);

        // East Wall
        defaultShader.setMat4("model", eastWallModel);
        eastWall->Draw(defaultShader, camera);

        // East Side Glass 1
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        defaultShader.use();

        defaultShader.setMat4("model", eastSideGlass1Model);
        eastSideGlass1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", eastSideGlass1_1Model);
        eastSideGlass1_1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", eastSideGlass2Model);
        eastSideGlass2->Draw(defaultShader, camera);

        defaultShader.setMat4("model", eastSideGlass2_1Model);
        eastSideGlass2_1->Draw(defaultShader, camera);

        // Roof
        defaultShader.setMat4("model", roofModel);
        roof->Draw(defaultShader, camera);

        // West Wall
        defaultShader.setMat4("model", westWallModel);
        westWall->Draw(defaultShader, camera);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);

        defaultShader.setMat4("model", longWall1Model);
        longWall1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", longWall2Model);
        longWall2->Draw(defaultShader, camera);

        defaultShader.setMat4("model", woodPanel1Model);
        woodPanel1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", woodPanel2Model);
        woodPanel2->Draw(defaultShader, camera);

        // Grid
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -3.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid1[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -3.00f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid2[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -2.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid3[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -2.00f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid4[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -1.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid5[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -1.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid6[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -0.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid7[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 0.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid8[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 0.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid9[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 1.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid10[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 1.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid11[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = westGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 2.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            westGrid12[i]->Draw(defaultShader, camera);
        }

        // EAST GRID
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -3.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid1[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -3.00f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid2[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -2.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid3[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -2.00f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid4[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -1.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid5[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -1.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid6[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, -0.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid7[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 0.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid8[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 0.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid9[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 1.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid10[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 1.5f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid11[i]->Draw(defaultShader, camera);
        }
        for (int i = 0; i < num_grid_west; i++) {
            mat4 tempModal = eastGridModals[i];
            tempModal = translate(tempModal, vec3(0.0f, 2.0f, 0.0f));

            defaultShader.setMat4("model", tempModal);
            eastGrid12[i]->Draw(defaultShader, camera);
        }
        
        // Table
        defaultShader.setMat4("model", tab1Model);
        Table1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", tab2Model);
        Table2->Draw(defaultShader, camera);

        defaultShader.setMat4("model", chair1Model);
        chair1->Draw(defaultShader, camera);

        defaultShader.setMat4("model", chair2Model);
        chair2->Draw(defaultShader, camera);

        lightShader.use();

        // Directional Light
        dirLightModel = mat4(1.0f);
        dirLightModel = translate(dirLightModel, dirLightPos);
        dirLightModel = scale(dirLightModel, vec3(0.1f, 0.1f, 0.1f));
        lightShader.setMat4("model", dirLightModel);
        lightShader.setVec4("lightColour", vec4(sunLight.colour, 1.0));
        sunLight.Draw(lightShader, camera);

        // Point Lights

        for (int i = 0; i < 20; i++) {
            pointModels[i] = mat4(1.0f);
            pointModels[i] = translate(pointModels[i], spotPos[i]);
            pointModels[i] = scale(pointModels[i], vec3(0.1f, 0.1f, 0.1f));

            lightShader.setMat4("model", pointModels[i]);
            lightShader.setVec4("lightColour", vec4(pointLights[i].colour, 1.0));

            pointLights[i].Draw(lightShader, camera);
        }

        // Spot Light
        droneLightModel = mat4(1.0f);
        droneLightModel = translate(droneLightModel, droneLightPos);
        droneLightModel = scale(droneLightModel, vec3(0.1f, 0.1f, 0.1f));

        lightShader.setMat4("model", droneLightModel);
        lightShader.setVec4("lightColour", vec4(drone_light.colour, 1.0));
        // drone_light.Draw(lightShader, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    // delete scene;
}

void processInput(GLFWwindow* window) {
    camera.movement_speed = 2.5f * deltaTime;

    camera.processInput(window);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        // Render wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.processMouseScroll(ypos);
}
