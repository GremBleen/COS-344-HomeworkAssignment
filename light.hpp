#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

using namespace glm;

class Light {
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    VAO vao;

    Light(vector<Vertex> vertices, vector<GLuint> indices) {
        this->vertices = vertices;
        this->indices = indices;
    }

    void Draw(Shader& shader, Camera& camera) {
        shader.use();
        vao.bind();

        shader.setVec3("cameraPosition", camera.position);
        camera.setShaderMat4(shader, "camMatrix");
        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void setupLight() {
        vao.bind();
        VBO VBO(vertices);
        EBO EBO(indices);

        vao.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // vertex positions

        vao.un_bind();
    }
};

class PointLight : public Light {
public:
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;

    float constant;
    float linear;
    float quadratic;

    PointLight(vec3 position, vec3 ambient, vec3 diffuse, vec3 specular, float constant, float linear, float quadratic, vector<Vertex> vertices, vector<GLuint> indices) : Light(vertices, indices) {
        this->position = position;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

        setupLight();
    }

    void Draw(Shader& shader, Camera& camera) {
        shader.use();
        vao.bind();

        shader.setVec3("cameraPosition", camera.position);
        camera.setShaderMat4(shader, "camMatrix");
        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void sendToShader(Shader& shader, int index) {
        shader.use();
        shader.setVec3("lightColour", colour);
        shader.setVec3("pointLights[" + to_string(index) + "].position", position);
        shader.setVec3("pointLights[" + to_string(index) + "].ambient", ambient);
        shader.setVec3("pointLights[" + to_string(index) + "].diffuse", diffuse);
        shader.setVec3("pointLights[" + to_string(index) + "].specular", specular);
        shader.setFloat("pointLights[" + to_string(index) + "].constant", constant);
        shader.setFloat("pointLights[" + to_string(index) + "].linear", linear);
        shader.setFloat("pointLights[" + to_string(index) + "].quadratic", quadratic);
    }
};

class DirectionalLight : public Light {
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vec3 position;
    vec3 direction;
    vec3 colour;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    DirectionalLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, vector<Vertex> vertices, vector<GLuint> indices) : Light(vertices, indices) {
        this->position = position;
        this->direction = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->vertices = vertices;
        this->indices = indices;

        setupLight();
    }

    void Draw(Shader& shader, Camera& camera) {
        shader.use();
        vao.bind();

        shader.setVec3("cameraPosition", camera.position);
        camera.setShaderMat4(shader, "camMatrix");
        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void sendToShader(Shader& shader) {
        shader.use();
        shader.setVec3("lightColour", colour);
        shader.setVec3("dirLight.direction", direction);
        shader.setVec3("dirLight.ambient", ambient);
        shader.setVec3("dirLight.diffuse", diffuse);
        shader.setVec3("dirLight.specular", specular);
    }
};

class SpotLight : public Light {
public:
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 colour;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    SpotLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, vector<Vertex> vertices, vector<GLuint> indices) : Light(vertices, indices) {
        this->position = position;
        this->direction = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;
        this->cutOff = cutOff;
        this->outerCutOff = outerCutOff;

        setupLight();
    }

    void Draw(Shader& shader, Camera& camera) {
        shader.use();
        vao.bind();

        shader.setVec3("cameraPosition", camera.position);
        camera.setShaderMat4(shader, "camMatrix");
        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void sendToShader(Shader& shader) {
        shader.use();
        shader.setVec3("lightColour", colour);
        shader.setVec3("spotLight.position", position);
        shader.setVec3("spotLight.direction", direction);
        shader.setVec3("spotLight.ambient", ambient);
        shader.setVec3("spotLight.diffuse", diffuse);
        shader.setVec3("spotLight.specular", specular);
        shader.setFloat("spotLight.constant", constant);
        shader.setFloat("spotLight.linear", linear);
        shader.setFloat("spotLight.quadratic", quadratic);
        shader.setFloat("spotLight.cutOff", cutOff);
        shader.setFloat("spotLight.outerCutOff", outerCutOff);
    }
};


#endif


