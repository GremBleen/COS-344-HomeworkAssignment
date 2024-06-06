#version 330 core

layout (location = 0) in vec3 vertexPosition;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * model * vec4(vertexPosition, 1.0f);
}