#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

using namespace glm;

class Mesh {
public:
    // Mesh data
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    VAO vao;

    mat4 modelMatrix;

    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        modelMatrix = mat4(1.0f);

        setupMesh();
    }

    void Draw(Shader& shader, Camera& camera) {
        // Bind appropriate textures
        shader.use();
        vao.bind();

        if (textures.size() == 0) {
            shader.setVec3("cameraPosition", camera.position);
            camera.setShaderMat4(shader, "camMatrix");

            glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
            vao.un_bind();
        }

        GLuint diffuseNr = 0;
        GLuint specularNr = 0;
        for (GLuint i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "diffuse")
                number = to_string(diffuseNr++);
            else if (name == "specular")
                number = to_string(specularNr++);

            // Now set the sampler to the correct texture unit
            textures[i].texUnit(shader, (name + number).c_str(), i);
            textures[i].bind();
        }
        shader.setVec3("cameraPosition", camera.position);
        camera.setShaderMat4(shader, "camMatrix");

        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
        vao.un_bind();        
    }
    // Render data

    void setupMesh() {
        vao.bind();
        VBO VBO(vertices);
        EBO EBO(indices);

        vao.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // vertex positions
        vao.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // vertex normals
        vao.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // vertex texture coords
        vao.LinkAttrib(VBO, 3, 4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Colour)); // vertex colours

        vao.un_bind();
    }
};

#endif


