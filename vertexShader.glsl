#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoords;

// out vec4 fragmentColor;
// out vec3 fragmentPos;
// out vec3 ambientColour; // White light for ambient and diffuse
// out vec3 normal;

out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){

    fragTexCoords = vertexTexCoords;

    // Output position of the vertex, in clip space : MVP * position
    gl_Position = proj * view * model * vec4(vertexPosition, 1.0);

    // gl_Position.xyz = vertexPosition;
    // gl_Position.w = 1.0;
    // fragmentColor = vertexColor;
    // fragmentPos = vertexPosition_modelspace;
    // ambientColour = vec3(1.0, 1.0, 1.0);
    // normal = vertexNormal_modelspace;
}