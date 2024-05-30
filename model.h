#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "shader.hpp"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"

class Model{
    public:
    vector<Mesh> meshes;

    Model(vector<Mesh> meshes){
        this->meshes = meshes;
    }

    void Draw(Shader& shader){
        for (GLuint i = 0; i < meshes.size(); i++){
            meshes[i].Draw(shader);
        }
    }

    // TODO: MAKE all shapes and models in here, like shape class
};

#endif