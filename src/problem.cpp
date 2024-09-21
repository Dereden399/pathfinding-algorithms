#include "problem.hpp"

Problem::Problem(int n, int m) {
    this->n = n;
    this->m = m;
    grid.resize(n);
    for (unsigned int x = 0; x < n; x++) {
        grid[x].reserve(m);
        for (unsigned int y = 0; y < m; y++) {
            grid[x].push_back(Cell(x, y));
        }
    }
    solution = {};
    start = nullptr;
    goal = nullptr;
}

void Problem::clearSolution() { solution = {}; };

void Problem::setCell(unsigned int x, unsigned int y, DrawMode mode) {
    switch (mode) {
        case DrawMode::WALL:
            grid[x][y].type = CellType::WALL;
            break;
        case DrawMode::FOREST:
            grid[x][y].type = CellType::FOREST;
            break;
        case DrawMode::WATER:
            grid[x][y].type = CellType::WATER;
            break;
        case DrawMode::PATH_GOAL:
            if (grid[x][y].type == CellType::WALL ||
                (start && start->x == x && start->y == y))
                break;
            goal = &grid[x][y];
            break;
        case DrawMode::PATH_START:
            if (grid[x][y].type == CellType::WALL ||
                (goal && goal->x == x && goal->y == y))
                break;
            start = &grid[x][y];
            break;
        default:
            grid[x][y].type = CellType::CLEAR;
            break;
    }
};

void Problem::clearCell(unsigned int x, unsigned int y) {
    grid[x][y].type = CellType::CLEAR;
    if (start && start->x == x && start->y == y) {
        start = nullptr;
    }
    if (goal && goal->x == x && goal->y == y) {
        goal = nullptr;
    }
};