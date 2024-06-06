#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>
#include<vector>

class EBO {
public:
    // ID reference of Elements Buffer Object
    GLuint id;
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(vector<GLuint>& indices) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    }

    // Binds the EBO
    void bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }
    // Unbinds the EBO
    void un_bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    // Deletes the EBO
    void deleteEBO() {
        glDeleteBuffers(1, &id);
    }
};

#endif