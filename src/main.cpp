#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include "renderer/context.hpp"
#include "problem.hpp"
#include "solver.hpp"

void processInput(GLFWwindow *window, Problem &problem, short &drawingMode,
                  std::unique_ptr<Solver> &solver);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void drawGrid(Context &ctx, Problem &problem);

int main() {
    std::cout << "Starting..." << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Application", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int SCREEN_WIDTH, SCREEN_HEIGHT;
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Context context = Context();
    std::shared_ptr<Shader> shader = context.setShaderProgramFiles(
        "shaders/vertex.glsl", "shaders/fragment.glsl");

    short drawingMode = 1;
    Problem problem = Problem(800 / 50, 600 / 50);
    std::unique_ptr<Solver> solver = std::make_unique<AStarSolver>();

    while (!glfwWindowShouldClose(window)) {
        processInput(window, problem, drawingMode, solver);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader->use();

        glm::mat4 view =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        shader->setUniform("view", view);
        auto proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 10.0f);
        shader->setUniform("projection", proj);

        drawGrid(context, problem);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, Problem &problem, short &drawingMode,
                  std::unique_ptr<Solver> &solver) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        drawingMode = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        drawingMode = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        drawingMode = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        solver = std::make_unique<AStarSolver>();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        problem = Problem(800 / 50, 600 / 50);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        drawingMode = 10;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        drawingMode = 11;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int x = xpos / 50;
        int y = (600 - ypos) / 50;
        if (x >= 800 / 50 || y >= 600 / 50) return;
        if (drawingMode == 10) {
            if (problem.grid[x][y] == 1 ||
                (problem.goal[0] == x && problem.goal[1] == y))
                return;
            problem.start[0] = x;
            problem.start[1] = y;
        } else if (drawingMode == 11) {
            if (problem.grid[x][y] == 1 ||
                (problem.start[0] == x && problem.start[1] == y))
                return;
            problem.goal[0] = x;
            problem.goal[1] = y;
        } else
            problem.grid[x][y] = drawingMode;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int x = xpos / 50;
        int y = (600 - ypos) / 50;
        if (x >= 800 / 50 || y >= 600 / 50) return;
        problem.grid[x][y] = 0;
        if (problem.goal[0] == x && problem.goal[1] == y) {
            problem.goal[0] = -1;
            problem.goal[1] = -1;
        }
        if (problem.start[0] == x && problem.start[1] == y) {
            problem.start[0] = -1;
            problem.start[1] = -1;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        solver->solved = false;
        problem.solution = {};
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (solver->solved) return;
        solver->solve(problem);
    }
}

void drawGrid(Context &ctx, Problem &problem) {
    for (int i = 0; i < 800; i += 50) {
        ctx.drawRectangle(i, 0.0f, 2.0f, 600.0f, 0,
                          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
    for (int i = 0; i < 600; i += 50) {
        ctx.drawRectangle(0.0f, i, 800.0f, 2.0f, 0,
                          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
    auto grid = problem.grid;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 0) {
                continue;
            }
            if (grid[i][j] == 1) {
                ctx.drawRectangle(i * 50, j * 50, 50, 50, 0,
                                  glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            }
            if (grid[i][j] == 2) {
                ctx.drawRectangle(i * 50, j * 50, 50, 50, 0,
                                  glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
            if (grid[i][j] == 3) {
                ctx.drawRectangle(i * 50, j * 50, 50, 50, 0,
                                  glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }
    if (problem.start[0] != -1 && problem.start[1] != -1) {
        ctx.drawRectangle(problem.start[0] * 50 + 10,
                          problem.start[1] * 50 + 10, 30, 30, 1,
                          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    if (problem.goal[0] != -1 && problem.goal[1] != -1) {
        ctx.drawRectangle(problem.goal[0] * 50 + 10, problem.goal[1] * 50 + 10,
                          30, 30, 1, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    }
    for (const auto &cell : problem.solution) {
        ctx.drawRectangle(cell[0] * 50 + 15, cell[1] * 50 + 15, 20, 20, 2,
                          glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}