#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include"VBO.hpp"

class VAO {
public:
    GLuint id;
    VAO() {
        glGenVertexArrays(1, &id);
    }

    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
        VBO.bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.un_bind();
    }
    void bind() {
        glBindVertexArray(id);
    }
    void un_bind() {
        glBindVertexArray(0);
    }
    void deleteVAO() {
        glDeleteVertexArrays(1, &id);
    }
};

#endif