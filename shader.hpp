#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Shader {
public:
    GLuint programID;

    Shader(const char* vertex_file_path, const char* fragment_file_path);
    GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

    void use();

    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;
    void setVec2(const string& name, const glm::vec2& value) const;
    void setVec3(const string& name, const glm::vec3& value) const;
    void setVec4(const string& name, const glm::vec4& value) const;
    void setMat2(const string& name, const glm::mat2& mat) const;
    void setMat3(const string& name, const glm::mat3& mat) const;
    void setMat4(const string& name, const glm::mat4& mat) const;
};
#endif
