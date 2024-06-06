#ifndef VBO_H
#define VBO_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

using namespace glm;

struct Vertex {
    // Position
    vec3 Position;
    // Normal
    vec3 Normal;
    // TexCoords
    vec2 TexCoords;
    // Colour
    vec4 Colour;
};

class VBO {
public:
    // Reference ID of the Vertex Buffer Object
    GLuint id;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(vector<Vertex>& vertices) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    }

    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }
    void un_bind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void deleteVBO() {
        glDeleteBuffers(1, &id);
    }
};

#endif