#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>

enum class CellType { CLEAR = 1, WALL = 100000, FOREST = 5, WATER = 10 };

enum class DrawMode { WALL, FOREST, WATER, PATH_START, PATH_GOAL };

constexpr glm::vec4 getCellColor(CellType& type) {
    switch (type) {
        case CellType::CLEAR:
            return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        case CellType::FOREST:
            return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        case CellType::WATER:
            return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        case CellType::WALL:
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        default:
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
};

#endif