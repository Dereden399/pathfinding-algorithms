#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>

enum class CellType { CLEAR = 1, WALL = 100000, FOREST = 5, WATER = 10 };

enum class DrawMode { WALL, FOREST, WATER, PATH_START, PATH_GOAL };

struct Cell {
    unsigned int x, y;
    bool visited = false;
    CellType type = CellType::CLEAR;
    Cell(unsigned int x_, unsigned int y_) : x(x_), y(y_) {};
};

constexpr glm::vec4 getCellColor(Cell& cell) {
    glm::vec4 col;
    switch (cell.type) {
        case CellType::CLEAR:
            col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case CellType::FOREST:
            col = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        case CellType::WATER:
            col = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
            break;
        case CellType::WALL:
            col = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        default:
            col = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            break;
    }
    if (cell.visited)
        col = glm::mix(col, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 0.5f);
    return col;
};

#endif