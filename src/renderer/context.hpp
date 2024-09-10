#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <memory>
#include "shader.hpp"

struct Vertex {
    float x, y, z;
    float u, v;
};

class Context {
    unsigned int baseVAO, baseVBO, baseEBO;
    std::shared_ptr<Shader> shader;
    Vertex baseVertices[4] = {{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
                              {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
                              {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
                              {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}};
    unsigned int baseIndices[6] = {0, 1, 2, 2, 3, 0};

   public:
    Context();
    ~Context();
    // copy constructors
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    // move constructors
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

    std::shared_ptr<Shader> setShaderProgramSource(
        const char* vertexShaderSource, const char* fragmentShaderSource);

    std::shared_ptr<Shader> setShaderProgramFiles(
        const char* vertexShaderPath, const char* fragmentShaderPath);

    void drawRectangle(float x, float y, float width, float height);
};

#endif