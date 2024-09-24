#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include "renderer/context.hpp"
#include "problem.hpp"
#include "solver.hpp"
#include "utils.hpp"

const double SOLVING_SPEED = 0.01;

void processInput(GLFWwindow *window, double time, Problem &problem,
                  DrawMode &drawingMode, std::unique_ptr<Solver> &solver);

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

    auto drawingMode = DrawMode::WALL;
    Problem problem = Problem(800 / 50, 600 / 50);
    std::unique_ptr<Solver> solver = std::make_unique<AStarSolver>();
    glfwSetWindowTitle(window, solver->getName().c_str());

    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, currentFrame, problem, drawingMode, solver);
        if (solver->solving &&
            currentFrame - solver->lastStepTime >= SOLVING_SPEED) {
            solver->lastStepTime = currentFrame;
            int res = solver->stepSolve(problem);
            glfwSetWindowTitle(window, (solver->getName() +
                                        ". Path cost: " + std::to_string(res))
                                           .c_str());
        }

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

void processInput(GLFWwindow *window, double time, Problem &problem,
                  DrawMode &drawingMode, std::unique_ptr<Solver> &solver) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        drawingMode = DrawMode::WALL;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        drawingMode = DrawMode::FOREST;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        drawingMode = DrawMode::WATER;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        drawingMode = DrawMode::PATH_START;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        drawingMode = DrawMode::PATH_GOAL;
    }
    // Next input operations are not available during solving
    if (solver->solving) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        solver = std::make_unique<AStarSolver>();
        glfwSetWindowTitle(window, solver->getName().c_str());
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        solver = std::make_unique<DijkstraSolver>();
        glfwSetWindowTitle(window, solver->getName().c_str());
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        problem = Problem(800 / 50, 600 / 50);
        solver->solved = false;
        solver->solving = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int x = xpos / 50;
        int y = (600 - ypos) / 50;
        if (x >= 800 / 50 || y >= 600 / 50 || x < 0 || y < 0) return;
        problem.setCell(x, y, drawingMode);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int x = xpos / 50;
        int y = (600 - ypos) / 50;
        if (x >= 800 / 50 || y >= 600 / 50 || x < 0 || y < 0) return;
        problem.clearCell(x, y);
    }
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        solver->solved = false;
        problem.restart();
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
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
    auto &grid = problem.grid;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            ctx.drawRectangle(i * 50, j * 50, 50, 50, 0,
                              getCellColor(grid[i][j]));
        }
    }
    if (problem.start) {
        ctx.drawRectangle(problem.start->x * 50 + 10,
                          problem.start->y * 50 + 10, 30, 30, 1,
                          glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    if (problem.goal) {
        ctx.drawRectangle(problem.goal->x * 50 + 10, problem.goal->y * 50 + 10,
                          30, 30, 1, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    }
    for (const auto cell : problem.solution) {
        int x = cell->x;
        int y = cell->y;
        ctx.drawRectangle(x * 50 + 15, y * 50 + 15, 20, 20, 2,
                          glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}