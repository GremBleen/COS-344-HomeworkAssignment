#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec4 vertexColour;

out vec3 fragPosition;
out vec3 fragNormal;
out vec4 fragColour;
out vec2 fragTexCoord;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    fragPosition = vec3(model * vec4(vertexPosition, 1.0f));
    fragNormal = mat3(transpose(inverse(model))) * vertexNormal;
    fragColour = vertexColour;
    fragTexCoord = vertexTexCoord;
    
    gl_Position = camMatrix * vec4(fragPosition, 1.0f);
}