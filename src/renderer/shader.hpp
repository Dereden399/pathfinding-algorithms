#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

struct Shader {
    unsigned int id;
    Shader(unsigned int id) : id(id) {}
    ~Shader() { glDeleteProgram(id); }
};

#endif